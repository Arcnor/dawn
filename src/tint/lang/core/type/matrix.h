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

#ifndef SRC_TINT_LANG_CORE_TYPE_MATRIX_H_
#define SRC_TINT_LANG_CORE_TYPE_MATRIX_H_

#include <string>

#include "tint/lang/core/type/vector.h"

// Forward declarations
namespace tint::core::type {
class Vector;
}  // namespace tint::core::type

namespace tint::core::type {

/// A matrix type
class Matrix final : public Castable<Matrix, Type> {
  public:
    /// Constructor
    /// @param column_type the type of a column of the matrix
    /// @param columns the number of columns in the matrix
    Matrix(const Vector* column_type, uint32_t columns);

    /// Destructor
    ~Matrix() override;

    /// @param other the other node to compare against
    /// @returns true if the this type is equal to @p other
    bool Equals(const UniqueNode& other) const override;

    /// @returns the type of the matrix
    const Type* type() const { return subtype_; }
    /// @returns the number of rows in the matrix
    uint32_t rows() const { return rows_; }
    /// @returns the number of columns in the matrix
    uint32_t columns() const { return columns_; }

    /// @returns the column-vector type of the matrix
    const Vector* ColumnType() const { return column_type_; }

    /// @returns the name for this type that closely resembles how it would be
    /// declared in WGSL.
    std::string FriendlyName() const override;

    /// @returns the size in bytes of the type. This may include tail padding.
    uint32_t Size() const override;

    /// @returns the alignment in bytes of the type. This may include tail
    /// padding.
    uint32_t Align() const override;

    /// @returns the number of bytes between columns of the matrix
    uint32_t ColumnStride() const;

    /// @copydoc Type::Elements
    TypeAndCount Elements(const Type* type_if_invalid = nullptr,
                          uint32_t count_if_invalid = 0) const override;

    /// @copydoc Type::Element
    const Vector* Element(uint32_t index) const override;

    /// @param ctx the clone context
    /// @returns a clone of this type
    Matrix* Clone(CloneContext& ctx) const override;

  private:
    const Type* const subtype_;
    const Vector* const column_type_;
    const uint32_t rows_;
    const uint32_t columns_;
};

}  // namespace tint::core::type

#endif  // SRC_TINT_LANG_CORE_TYPE_MATRIX_H_
