// Copyright 2022 The Dawn & Tint Authors
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

#ifndef SRC_TINT_UTILS_CONTAINERS_HASHMAP_BASE_H_
#define SRC_TINT_UTILS_CONTAINERS_HASHMAP_BASE_H_

#include <algorithm>
#include <functional>
#include <optional>
#include <tuple>
#include <utility>

#include "tint/utils/containers/vector.h"
#include "tint/utils/ice/ice.h"
#include "tint/utils/math/hash.h"
#include "tint/utils/traits/traits.h"

#define TINT_ASSERT_ITERATORS_NOT_INVALIDATED

namespace tint {

/// Action taken by a map mutation
enum class MapAction {
    /// A new entry was added to the map
    kAdded,
    /// A existing entry in the map was replaced
    kReplaced,
    /// No action was taken as the map already contained an entry with the given key
    kKeptExisting,
};

/// KeyValue is a key-value pair.
template <typename KEY, typename VALUE>
struct KeyValue {
    /// The key type
    using Key = KEY;
    /// The value type
    using Value = VALUE;

    /// The key
    Key key;

    /// The value
    Value value;

    /// Equality operator
    /// @param other the RHS of the operator
    /// @returns true if both the key and value of this KeyValue are equal to the key and value
    /// of @p other
    template <typename K, typename V>
    bool operator==(const KeyValue<K, V>& other) const {
        return key == other.key && value == other.value;
    }

    /// Inequality operator
    /// @param other the RHS of the operator
    /// @returns true if either the key and value of this KeyValue are not equal to the key and
    /// value of @p other
    template <typename K, typename V>
    bool operator!=(const KeyValue<K, V>& other) const {
        return *this != other;
    }
};

/// KeyValueRef is a pair of references to a key and value.
/// #key is always a const reference.
/// #value is always a const reference if @tparam VALUE_IS_CONST is true, otherwise a non-const
/// reference.
template <typename KEY, typename VALUE, bool VALUE_IS_CONST>
struct KeyValueRef {
    /// The reference to key type
    using KeyRef = const KEY&;
    /// The reference to value type
    using ValueRef = std::conditional_t<VALUE_IS_CONST, const VALUE&, VALUE&>;

    /// The reference to the key
    KeyRef key;

    /// The reference to the value
    ValueRef value;

    /// @returns a KeyValue<KEY, VALUE> with the referenced key and value
    operator KeyValue<KEY, VALUE>() const { return {key, value}; }
};

/// Writes the KeyValue to the stream.
/// @param out the stream to write to
/// @param key_value the KeyValue to write
/// @returns out so calls can be chained
template <typename STREAM,
          typename KEY,
          typename VALUE,
          typename = traits::EnableIfIsOStream<STREAM>>
auto& operator<<(STREAM& out, const KeyValue<KEY, VALUE>& key_value) {
    return out << "[" << key_value.key << ": " << key_value.value << "]";
}

/// A base class for Hashmap and Hashset that uses a robin-hood hashing algorithm.
/// @see the fantastic tutorial: https://programming.guide/robin-hood-hashing.html
template <typename KEY,
          typename VALUE,
          size_t N,
          typename HASH = Hasher<KEY>,
          typename EQUAL = EqualTo<KEY>>
class HashmapBase {
    static constexpr bool ValueIsVoid = std::is_same_v<VALUE, void>;

  public:
    /// The key type
    using Key = KEY;
    /// The value type
    using Value = VALUE;
    /// The entry type for the map.
    /// This is:
    /// - Key when Value is void (used by Hashset)
    /// - KeyValue<Key, Value> when Value is not void (used by Hashmap)
    using Entry = std::conditional_t<ValueIsVoid, Key, KeyValue<Key, Value>>;

    /// A reference to an entry in the map.
    /// This is:
    /// - const Key& when Value is void (used by Hashset)
    /// - KeyValueRef<Key, Value> when Value is not void (used by Hashmap)
    template <bool IS_CONST>
    using EntryRef = std::conditional_t<
        ValueIsVoid,
        const Key&,
        KeyValueRef<Key, std::conditional_t<ValueIsVoid, bool, Value>, IS_CONST>>;

    /// STL-friendly alias to Entry. Used by gmock.
    using value_type = Entry;

  private:
    /// @returns the key from an entry
    static const Key& KeyOf(const Entry& entry) {
        if constexpr (ValueIsVoid) {
            return entry;
        } else {
            return entry.key;
        }
    }

    /// @returns a pointer to the value from an entry.
    static Value* ValueOf(Entry& entry) {
        if constexpr (ValueIsVoid) {
            return nullptr;  // Hashset only has keys
        } else {
            return &entry.value;
        }
    }

    /// A slot is a single entry in the underlying vector.
    /// A slot can either be empty or filled with a value. If the slot is empty, #hash and #distance
    /// will be zero.
    struct Slot {
        template <typename K>
        bool Equals(size_t key_hash, K&& key) const {
            return key_hash == hash && EQUAL()(std::forward<K>(key), KeyOf(*entry));
        }

        /// The slot value. If this does not contain a value, then the slot is vacant.
        std::optional<Entry> entry;
        /// The precomputed hash of value.
        size_t hash = 0;
        size_t distance = 0;
    };

    /// The target length of the underlying vector length in relation to the number of entries in
    /// the map, expressed as a percentage. For example a value of `150` would mean there would be
    /// at least 50% more slots than the number of map entries.
    static constexpr size_t kRehashFactor = 150;

    /// @returns the target slot vector size to hold `n` map entries.
    static constexpr size_t NumSlots(size_t count) { return (count * kRehashFactor) / 100; }

    /// The fixed-size slot vector length, based on N and kRehashFactor.
    static constexpr size_t kNumFixedSlots = NumSlots(N);

    /// The minimum number of slots for the map.
    static constexpr size_t kMinSlots = std::max<size_t>(kNumFixedSlots, 4);

  public:
    /// Iterator for entries in the map.
    /// Iterators are invalidated if the map is modified.
    template <bool IS_CONST>
    class IteratorT {
      public:
        /// @returns the value pointed to by this iterator
        EntryRef<IS_CONST> operator->() const {
#ifdef TINT_ASSERT_ITERATORS_NOT_INVALIDATED
            TINT_ASSERT(map.Generation() == initial_generation &&
                        "iterator invalidated by container modification");
#endif
            return *this;
        }

        /// @returns a reference to the value at the iterator
        EntryRef<IS_CONST> operator*() const {
#ifdef TINT_ASSERT_ITERATORS_NOT_INVALIDATED
            TINT_ASSERT(map.Generation() == initial_generation &&
                        "iterator invalidated by container modification");
#endif
            auto& ref = current->entry.value();
            if constexpr (ValueIsVoid) {
                return ref;
            } else {
                return {ref.key, ref.value};
            }
        }

        /// Increments the iterator
        /// @returns this iterator
        IteratorT& operator++() {
#ifdef TINT_ASSERT_ITERATORS_NOT_INVALIDATED
            TINT_ASSERT(map.Generation() == initial_generation &&
                        "iterator invalidated by container modification");
#endif
            if (current == end) {
                return *this;
            }
            ++current;
            SkipToNextValue();
            return *this;
        }

        /// Equality operator
        /// @param other the other iterator to compare this iterator to
        /// @returns true if this iterator is equal to other
        bool operator==(const IteratorT& other) const {
#ifdef TINT_ASSERT_ITERATORS_NOT_INVALIDATED
            TINT_ASSERT(map.Generation() == initial_generation &&
                        "iterator invalidated by container modification");
#endif
            return current == other.current;
        }

        /// Inequality operator
        /// @param other the other iterator to compare this iterator to
        /// @returns true if this iterator is not equal to other
        bool operator!=(const IteratorT& other) const {
#ifdef TINT_ASSERT_ITERATORS_NOT_INVALIDATED
            TINT_ASSERT(map.Generation() == initial_generation &&
                        "iterator invalidated by container modification");
#endif
            return current != other.current;
        }

      private:
        /// Friend class
        friend class HashmapBase;

        using SLOT = std::conditional_t<IS_CONST, const Slot, Slot>;

        IteratorT(VectorIterator<SLOT> c,
                  VectorIterator<SLOT> e,
                  [[maybe_unused]] const HashmapBase& m)
            : current(std::move(c)),
              end(std::move(e))
#ifdef TINT_ASSERT_ITERATORS_NOT_INVALIDATED
              ,
              map(m),
              initial_generation(m.Generation())
#endif
        {
            SkipToNextValue();
        }

        /// Moves the iterator forward, stopping at the next slot that is not empty.
        void SkipToNextValue() {
            while (current != end && !current->entry.has_value()) {
                ++current;
            }
        }

        VectorIterator<SLOT> current;  /// The slot the iterator is pointing to
        VectorIterator<SLOT> end;      /// One past the last slot in the map

#ifdef TINT_ASSERT_ITERATORS_NOT_INVALIDATED
        const HashmapBase& map;     /// The hashmap that is being iterated over.
        size_t initial_generation;  /// The generation ID when the iterator was created.
#endif
    };

    /// An immutable key and mutable value iterator
    using Iterator = IteratorT</*IS_CONST*/ false>;

    /// An immutable key and value iterator
    using ConstIterator = IteratorT</*IS_CONST*/ true>;

    /// Constructor
    HashmapBase() { slots_.Resize(kMinSlots); }

    /// Copy constructor
    /// @param other the other HashmapBase to copy
    HashmapBase(const HashmapBase& other) = default;

    /// Move constructor
    /// @param other the other HashmapBase to move
    HashmapBase(HashmapBase&& other) = default;

    /// Destructor
    ~HashmapBase() { Clear(); }

    /// Copy-assignment operator
    /// @param other the other HashmapBase to copy
    /// @returns this so calls can be chained
    HashmapBase& operator=(const HashmapBase& other) = default;

    /// Move-assignment operator
    /// @param other the other HashmapBase to move
    /// @returns this so calls can be chained
    HashmapBase& operator=(HashmapBase&& other) = default;

    /// Removes all entries from the map.
    void Clear() {
        slots_.Clear();  // Destructs all entries
        slots_.Resize(kMinSlots);
        count_ = 0;
        generation_++;
    }

    /// Removes an entry from the map.
    /// @param key the entry key.
    /// @returns true if an entry was removed.
    bool Remove(const Key& key) {
        const auto [found, start] = IndexOf(key);
        if (!found) {
            return false;
        }

        // Shuffle the entries backwards until we either find a free slot, or a slot that has zero
        // distance.
        Slot* prev = nullptr;

        const auto count = slots_.Length();
        for (size_t distance = 0, index = start; distance < count; distance++) {
            auto& slot = slots_[index];
            if (prev) {
                // note: `distance == 0` also includes empty slots.
                if (slot.distance == 0) {
                    // Clear the previous slot, and stop shuffling.
                    *prev = {};
                    break;
                }
                // Shuffle the slot backwards.
                prev->entry = std::move(slot.entry);
                prev->hash = slot.hash;
                prev->distance = slot.distance - 1;
            }
            prev = &slot;

            index = (index == count - 1) ? 0 : index + 1;
        }

        // Entry was removed.
        count_--;
        generation_++;

        return true;
    }

    /// Checks whether an entry exists in the map
    /// @param key the key to search for.
    /// @returns true if the map contains an entry with the given value.
    bool Contains(const Key& key) const {
        const auto [found, _] = IndexOf(key);
        return found;
    }

    /// Pre-allocates memory so that the map can hold at least `capacity` entries.
    /// @param capacity the new capacity of the map.
    void Reserve(size_t capacity) {
        // Calculate the number of slots required to hold `capacity` entries.
        const size_t num_slots = std::max(NumSlots(capacity), kMinSlots);
        if (slots_.Length() >= num_slots) {
            // Already have enough slots.
            return;
        }

        // Move all the values out of the map and into a vector.
        Vector<Entry, N> entries;
        entries.Reserve(count_);
        for (auto& slot : slots_) {
            if (slot.entry.has_value()) {
                entries.Push(std::move(slot.entry.value()));
            }
        }

        // Clear the map, grow the number of slots.
        Clear();
        slots_.Resize(num_slots);

        // As the number of slots has grown, the slot indices will have changed from before, so
        // re-add all the entries back into the map.
        for (auto& entry : entries) {
            if constexpr (ValueIsVoid) {
                struct NoValue {};
                Put<PutMode::kAdd>(std::move(entry), NoValue{});
            } else {
                Put<PutMode::kAdd>(std::move(entry.key), std::move(entry.value));
            }
        }
    }

    /// @returns the number of entries in the map.
    size_t Count() const { return count_; }

    /// @returns true if the map contains no entries.
    bool IsEmpty() const { return count_ == 0; }

    /// @returns a monotonic counter which is incremented whenever the map is mutated.
    size_t Generation() const { return generation_; }

    /// @returns an immutable iterator to the start of the map.
    ConstIterator begin() const { return ConstIterator{slots_.begin(), slots_.end(), *this}; }

    /// @returns an immutable iterator to the end of the map.
    ConstIterator end() const { return ConstIterator{slots_.end(), slots_.end(), *this}; }

    /// @returns an iterator to the start of the map.
    Iterator begin() { return Iterator{slots_.begin(), slots_.end(), *this}; }

    /// @returns an iterator to the end of the map.
    Iterator end() { return Iterator{slots_.end(), slots_.end(), *this}; }

    /// A debug function for checking that the map is in good health.
    /// Asserts if the map is corrupted.
    void ValidateIntegrity() const {
        size_t num_alive = 0;
        for (size_t slot_idx = 0; slot_idx < slots_.Length(); slot_idx++) {
            const auto& slot = slots_[slot_idx];
            if (slot.entry.has_value()) {
                num_alive++;
                auto const [index, hash] = Hash(KeyOf(*slot.entry));
                TINT_ASSERT(hash == slot.hash);
                TINT_ASSERT(slot_idx == Wrap(index + slot.distance));
            }
        }
        TINT_ASSERT(num_alive == count_);
    }

  protected:
    /// The behaviour of Put() when an entry already exists with the given key.
    enum class PutMode {
        /// Do not replace existing entries with the new value.
        kAdd,
        /// Replace existing entries with the new value.
        kReplace,
    };

    /// Result of Put()
    struct PutResult {
        /// Whether the insert replaced or added a new entry to the map.
        MapAction action = MapAction::kAdded;
        /// A pointer to the inserted entry value.
        Value* value = nullptr;

        /// @returns true if the entry was added to the map, or an existing entry was replaced.
        operator bool() const { return action != MapAction::kKeptExisting; }
    };

    /// The common implementation for Add() and Replace()
    /// @param key the key of the entry to add to the map.
    /// @param value the value of the entry to add to the map.
    /// @returns A PutResult describing the result of the insertion
    template <PutMode MODE, typename K, typename V>
    PutResult Put(K&& key, V&& value) {
        // Ensure the map can fit a new entry
        if (ShouldRehash(count_ + 1)) {
            Reserve((count_ + 1) * 2);
        }

        const auto hash = Hash(key);

        auto make_entry = [&] {
            if constexpr (ValueIsVoid) {
                return std::forward<K>(key);
            } else {
                return Entry{std::forward<K>(key), std::forward<V>(value)};
            }
        };

        const auto count = slots_.Length();
        for (size_t distance = 0, index = hash.scan_start; distance < count; distance++) {
            auto& slot = slots_[index];
            if (!slot.entry.has_value()) {
                // Found an empty slot.
                // Place value directly into the slot, and we're done.
                slot.entry.emplace(make_entry());
                slot.hash = hash.code;
                slot.distance = distance;
                count_++;
                generation_++;
                return PutResult{MapAction::kAdded, ValueOf(*slot.entry)};
            }

            // Slot has an entry

            if (slot.Equals(hash.code, key)) {
                // Slot is equal to value. Replace or preserve?
                if constexpr (MODE == PutMode::kReplace) {
                    slot.entry = make_entry();
                    generation_++;
                    return PutResult{MapAction::kReplaced, ValueOf(*slot.entry)};
                } else {
                    return PutResult{MapAction::kKeptExisting, ValueOf(*slot.entry)};
                }
            }

            if (slot.distance < distance) {
                // Existing slot has a closer distance than the value we're attempting to insert.
                // Steal from the rich!
                // Move the current slot to a temporary (evicted), and put the value into the slot.
                Slot evicted{make_entry(), hash.code, distance};
                std::swap(evicted, slot);

                // Find a new home for the evicted slot.
                evicted.distance++;  // We've already swapped at index.
                InsertShuffle(Wrap(index + 1), std::move(evicted));

                count_++;
                generation_++;
                return PutResult{MapAction::kAdded, ValueOf(*slot.entry)};
            }

            index = (index == count - 1) ? 0 : index + 1;
        }

        TINT_ICE() << "HashmapBase::Put() looped entire map without finding a slot";
        return PutResult{};
    }

    /// HashResult is the return value of Hash()
    struct HashResult {
        /// The target (zero-distance) slot index for the key.
        size_t scan_start;
        /// The calculated hash code of the key.
        size_t code;
    };

    /// @param key the key to hash
    /// @returns a tuple holding the target slot index for the given value, and the hash of the
    /// value, respectively.
    template <typename K>
    HashResult Hash(K&& key) const {
        size_t hash = HASH()(std::forward<K>(key));
        size_t index = Wrap(hash);
        return {index, hash};
    }

    /// Looks for the key in the map.
    /// @param key the key to search for.
    /// @returns a tuple holding a boolean representing whether the key was found in the map, and
    /// if found, the index of the slot that holds the key.
    template <typename K>
    std::tuple<bool, size_t> IndexOf(K&& key) const {
        const auto hash = Hash(key);
        const auto count = slots_.Length();
        for (size_t distance = 0, index = hash.scan_start; distance < count; distance++) {
            auto& slot = slots_[index];
            if (!slot.entry.has_value()) {
                return {/* found */ false, /* index */ 0};
            }
            if (slot.Equals(hash.code, key)) {
                return {/* found */ true, index};
            }
            if (slot.distance < distance) {
                // If the slot distance is less than the current probe distance, then the slot
                // must be for entry that has an index that comes after key. In this situation,
                // we know that the map does not contain the key, as it would have been found
                // before this slot. The "Lookup" section of
                // https://programming.guide/robin-hood-hashing.html suggests that the condition
                // should inverted, but this is wrong.
                return {/* found */ false, /* index */ 0};
            }
            index = (index == count - 1) ? 0 : index + 1;
        }

        TINT_ICE() << "HashmapBase::IndexOf() looped entire map without finding a slot";
        return {/* found */ false, /* index */ 0};
    }

    /// Shuffles slots for an insertion that has been placed one slot before `start`.
    /// @param start the index of the first slot to start shuffling.
    /// @param evicted the slot content that was evicted for the insertion.
    void InsertShuffle(size_t start, Slot&& evicted) {
        const auto count = slots_.Length();
        for (size_t distance = 0, index = start; distance < count; distance++) {
            auto& slot = slots_[index];

            if (!slot.entry.has_value()) {
                // Empty slot found for evicted.
                slot = std::move(evicted);
                return;  //  We're done.
            }

            if (slot.distance < evicted.distance) {
                // Occupied slot has shorter distance to evicted.
                // Swap slot and evicted.
                std::swap(slot, evicted);
            }

            // evicted moves further from the target slot...
            evicted.distance++;

            index = (index == count - 1) ? 0 : index + 1;
        }
    }

    /// @param count the number of new entries in the map
    /// @returns true if the map should grow the slot vector, and rehash the items.
    bool ShouldRehash(size_t count) const { return NumSlots(count) > slots_.Length(); }

    /// @param index an input value
    /// @returns the input value modulo the number of slots.
    size_t Wrap(size_t index) const { return index % slots_.Length(); }

    /// The vector of slots. The vector length is equal to its capacity.
    Vector<Slot, kNumFixedSlots> slots_;

    /// The number of entries in the map.
    size_t count_ = 0;

    /// Counter that's incremented with each modification to the map.
    size_t generation_ = 0;
};

}  // namespace tint

#endif  // SRC_TINT_UTILS_CONTAINERS_HASHMAP_BASE_H_
