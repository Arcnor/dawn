// Copyright 2021 The Dawn & Tint Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SRC_TINT_UTILS_MATH_HASH_H_
#define SRC_TINT_UTILS_MATH_HASH_H_

#include <stdint.h>
#include <cstdio>
#include <functional>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include "tint/utils/math/crc32.h"

namespace tint {
namespace detail {

/// Helper for obtaining a seed bias value for HashCombine with a bit-width
/// dependent on the size of size_t.
template <int SIZE_OF_SIZE_T>
struct HashCombineOffset {};

/// Specialization of HashCombineOffset for size_t == 4.
template <>
struct HashCombineOffset<4> {
    /// @returns the seed bias value for HashCombine()
    static constexpr inline uint32_t value() {
        constexpr uint32_t base = 0x7f4a7c16;
#ifdef TINT_HASH_SEED
        return base ^ static_cast<uint32_t>(TINT_HASH_SEED);
#endif
        return base;
    }
};

/// Specialization of HashCombineOffset for size_t == 8.
template <>
struct HashCombineOffset<8> {
    /// @returns the seed bias value for HashCombine()
    static constexpr inline uint64_t value() {
        constexpr uint64_t base = 0x9e3779b97f4a7c16;
#ifdef TINT_HASH_SEED
        return base ^ static_cast<uint64_t>(TINT_HASH_SEED);
#endif
        return base;
    }
};

template <typename T, typename = void>
struct HasHashCodeMember : std::false_type {};

template <typename T>
struct HasHashCodeMember<
    T,
    std::enable_if_t<std::is_member_function_pointer_v<decltype(&T::HashCode)>>> : std::true_type {
};

}  // namespace detail

/// Forward declarations (see below)
template <typename... ARGS>
size_t Hash(const ARGS&... values);

template <typename... ARGS>
size_t HashCombine(size_t hash, const ARGS&... values);

/// A STL-compatible hasher that does a more thorough job than most implementations of std::hash.
/// Hasher has been optimized for a better quality hash at the expense of increased computation
/// costs.
/// Hasher is specialized for various core Tint data types. The default implementation will use a
/// `size_t HashCode()` method on the `T` type, and will fallback to `std::hash<T>` if
/// `T::HashCode` is missing.
template <typename T>
struct Hasher {
    /// @param value the value to hash
    /// @returns a hash of the value
    size_t operator()(const T& value) const {
        if constexpr (detail::HasHashCodeMember<T>::value) {
            return value.HashCode();
        } else {
            return std::hash<T>()(value);
        }
    }
};

/// Hasher specialization for pointers
/// std::hash<T*> typically uses a reinterpret of the pointer to a size_t.
/// As most pointers a 4 or 16 byte aligned, this usually results in the LSBs of the hash being 0,
/// resulting in bad hashes for hashtables. This implementation mixes up those LSBs.
template <typename T>
struct Hasher<T*> {
    /// @param ptr the pointer to hash
    /// @returns a hash of the pointer
    size_t operator()(T* ptr) const {
        auto hash = std::hash<T*>()(ptr);
#ifdef TINT_HASH_SEED
        hash ^= static_cast<uint32_t>(TINT_HASH_SEED);
#endif
        return hash ^ (hash >> 4);
    }
};

/// Hasher specialization for std::vector
template <typename T>
struct Hasher<std::vector<T>> {
    /// @param vector the vector to hash
    /// @returns a hash of the vector
    size_t operator()(const std::vector<T>& vector) const {
        auto hash = Hash(vector.size());
        for (auto& el : vector) {
            hash = HashCombine(hash, el);
        }
        return hash;
    }
};

/// Hasher specialization for std::tuple
template <typename... TYPES>
struct Hasher<std::tuple<TYPES...>> {
    /// @param tuple the tuple to hash
    /// @returns a hash of the tuple
    size_t operator()(const std::tuple<TYPES...>& tuple) const {
        return std::apply(Hash<TYPES...>, tuple);
    }
};

/// Hasher specialization for std::pair
template <typename A, typename B>
struct Hasher<std::pair<A, B>> {
    /// @param tuple the tuple to hash
    /// @returns a hash of the tuple
    size_t operator()(const std::pair<A, B>& tuple) const { return std::apply(Hash<A, B>, tuple); }
};

/// Hasher specialization for std::variant
template <typename... TYPES>
struct Hasher<std::variant<TYPES...>> {
    /// @param variant the variant to hash
    /// @returns a hash of the tuple
    size_t operator()(const std::variant<TYPES...>& variant) const {
        return std::visit([](auto&& val) { return Hash(val); }, variant);
    }
};

/// Hasher specialization for std::string, which also supports hashing of const char* and
/// std::string_view without first constructing a std::string.
template <>
struct Hasher<std::string> {
    /// @param str the string to hash
    /// @returns a hash of the string
    size_t operator()(const std::string& str) const {
        return std::hash<std::string_view>()(std::string_view(str));
    }

    /// @param str the string to hash
    /// @returns a hash of the string
    size_t operator()(const char* str) const {
        return std::hash<std::string_view>()(std::string_view(str));
    }

    /// @param str the string to hash
    /// @returns a hash of the string
    size_t operator()(const std::string_view& str) const {
        return std::hash<std::string_view>()(str);
    }
};

/// @param args the arguments to hash
/// @returns a hash of the variadic list of arguments.
///          The returned hash is dependent on the order of the arguments.
template <typename... ARGS>
size_t Hash(const ARGS&... args) {
    if constexpr (sizeof...(ARGS) == 0) {
        return 0;
    } else if constexpr (sizeof...(ARGS) == 1) {
        using T = std::tuple_element_t<0, std::tuple<ARGS...>>;
        return Hasher<T>()(args...);
    } else {
        size_t hash = 102931;  // seed with an arbitrary prime
        return HashCombine(hash, args...);
    }
}

/// @param hash the hash value to combine with
/// @param values the values to hash
/// @returns a hash of the variadic list of arguments.
///          The returned hash is dependent on the order of the arguments.
template <typename... ARGS>
size_t HashCombine(size_t hash, const ARGS&... values) {
    constexpr size_t offset = tint::detail::HashCombineOffset<sizeof(size_t)>::value();
    ((hash ^= Hash(values) + (offset ^ (hash >> 2))), ...);
    return hash;
}

/// A STL-compatible equal_to implementation that specializes for types.
template <typename T>
struct EqualTo {
    /// @param lhs the left hand side value
    /// @param rhs the right hand side value
    /// @returns true if the two values are equal
    constexpr bool operator()(const T& lhs, const T& rhs) const {
        return std::equal_to<T>()(lhs, rhs);
    }
};

/// A specialization for EqualTo for std::string, which supports additional comparision with
/// std::string_view and const char*.
template <>
struct EqualTo<std::string> {
    /// @param lhs the left hand side value
    /// @param rhs the right hand side value
    /// @returns true if the two values are equal
    bool operator()(const std::string& lhs, const std::string& rhs) const { return lhs == rhs; }

    /// @param lhs the left hand side value
    /// @param rhs the right hand side value
    /// @returns true if the two values are equal
    bool operator()(const std::string& lhs, const char* rhs) const { return lhs == rhs; }

    /// @param lhs the left hand side value
    /// @param rhs the right hand side value
    /// @returns true if the two values are equal
    bool operator()(const std::string& lhs, std::string_view rhs) const { return lhs == rhs; }

    /// @param lhs the left hand side value
    /// @param rhs the right hand side value
    /// @returns true if the two values are equal
    bool operator()(const char* lhs, const std::string& rhs) const { return lhs == rhs; }

    /// @param lhs the left hand side value
    /// @param rhs the right hand side value
    /// @returns true if the two values are equal
    bool operator()(std::string_view lhs, const std::string& rhs) const { return lhs == rhs; }
};

/// Wrapper for a hashable type enabling the wrapped value to be used as a key
/// for an unordered_map or unordered_set.
template <typename T>
struct UnorderedKeyWrapper {
    /// The wrapped value
    T value;
    /// The hash of value
    size_t hash;

    /// Constructor
    /// @param v the value to wrap
    explicit UnorderedKeyWrapper(const T& v) : value(v), hash(Hash(v)) {}

    /// Move constructor
    /// @param v the value to wrap
    explicit UnorderedKeyWrapper(T&& v) : value(std::move(v)), hash(Hash(value)) {}

    /// @returns true if this wrapper comes before other
    /// @param other the RHS of the operator
    bool operator<(const UnorderedKeyWrapper& other) const { return hash < other.hash; }

    /// @returns true if this wrapped value is equal to the other wrapped value
    /// @param other the RHS of the operator
    bool operator==(const UnorderedKeyWrapper& other) const { return value == other.value; }
};

}  // namespace tint

namespace std {

/// Custom std::hash specialization for tint::UnorderedKeyWrapper
template <typename T>
class hash<tint::UnorderedKeyWrapper<T>> {
  public:
    /// @param w the UnorderedKeyWrapper
    /// @return the hash value
    inline std::size_t operator()(const tint::UnorderedKeyWrapper<T>& w) const { return w.hash; }
};

}  // namespace std

#endif  // SRC_TINT_UTILS_MATH_HASH_H_
