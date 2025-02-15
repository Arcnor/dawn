// Copyright 2023 The Dawn & Tint Authors
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

#include "tint/lang/spirv/writer/raise/handle_matrix_arithmetic.h"

#include <utility>

#include "tint/lang/core/fluent_types.h"
#include "tint/lang/core/ir/builder.h"
#include "tint/lang/core/ir/module.h"
#include "tint/lang/core/ir/validator.h"
#include "tint/lang/core/type/matrix.h"
#include "tint/lang/spirv/ir/builtin_call.h"
#include "tint/utils/ice/ice.h"

using namespace tint::core::number_suffixes;  // NOLINT
using namespace tint::core::fluent_types;     // NOLINT

namespace tint::spirv::writer::raise {

namespace {

void Run(core::ir::Module& ir) {
    core::ir::Builder b{ir};

    // Find the instructions that need to be modified.
    Vector<core::ir::Binary*, 4> binary_worklist;
    Vector<core::ir::Convert*, 4> convert_worklist;
    for (auto* inst : ir.instructions.Objects()) {
        if (!inst->Alive()) {
            continue;
        }
        if (auto* binary = inst->As<core::ir::Binary>()) {
            TINT_ASSERT(binary->Operands().Length() == 2);
            if (binary->LHS()->Type()->Is<core::type::Matrix>() ||
                binary->RHS()->Type()->Is<core::type::Matrix>()) {
                binary_worklist.Push(binary);
            }
        } else if (auto* convert = inst->As<core::ir::Convert>()) {
            if (convert->Result()->Type()->Is<core::type::Matrix>()) {
                convert_worklist.Push(convert);
            }
        }
    }

    // Replace the matrix arithmetic instructions that we found.
    for (auto* binary : binary_worklist) {
        auto* lhs = binary->LHS();
        auto* rhs = binary->RHS();
        auto* lhs_ty = lhs->Type();
        auto* rhs_ty = rhs->Type();
        auto* ty = binary->Result()->Type();

        // Helper to replace the instruction with a new one.
        auto replace = [&](core::ir::Instruction* inst) {
            if (auto name = ir.NameOf(binary)) {
                ir.SetName(inst->Result(), name);
            }
            binary->Result()->ReplaceAllUsesWith(inst->Result());
            binary->ReplaceWith(inst);
            binary->Destroy();
        };

        // Helper to replace the instruction with a column-wise operation.
        auto column_wise = [&](auto op) {
            auto* mat = ty->As<core::type::Matrix>();
            Vector<core::ir::Value*, 4> args;
            for (uint32_t col = 0; col < mat->columns(); col++) {
                b.InsertBefore(binary, [&] {
                    auto* lhs_col = b.Access(mat->ColumnType(), lhs, u32(col));
                    auto* rhs_col = b.Access(mat->ColumnType(), rhs, u32(col));
                    auto* add = b.Binary(op, mat->ColumnType(), lhs_col, rhs_col);
                    args.Push(add->Result());
                });
            }
            replace(b.Construct(ty, std::move(args)));
        };

        switch (binary->Op()) {
            case core::ir::BinaryOp::kAdd:
                column_wise(core::ir::BinaryOp::kAdd);
                break;
            case core::ir::BinaryOp::kSubtract:
                column_wise(core::ir::BinaryOp::kSubtract);
                break;
            case core::ir::BinaryOp::kMultiply:
                // Select the SPIR-V intrinsic that corresponds to the operation being performed.
                if (lhs_ty->Is<core::type::Matrix>()) {
                    if (rhs_ty->Is<core::type::Scalar>()) {
                        replace(b.Call<spirv::ir::BuiltinCall>(
                            ty, spirv::BuiltinFn::kMatrixTimesScalar, lhs, rhs));
                    } else if (rhs_ty->Is<core::type::Vector>()) {
                        replace(b.Call<spirv::ir::BuiltinCall>(
                            ty, spirv::BuiltinFn::kMatrixTimesVector, lhs, rhs));
                    } else if (rhs_ty->Is<core::type::Matrix>()) {
                        replace(b.Call<spirv::ir::BuiltinCall>(
                            ty, spirv::BuiltinFn::kMatrixTimesMatrix, lhs, rhs));
                    }
                } else {
                    if (lhs_ty->Is<core::type::Scalar>()) {
                        replace(b.Call<spirv::ir::BuiltinCall>(
                            ty, spirv::BuiltinFn::kMatrixTimesScalar, rhs, lhs));
                    } else if (lhs_ty->Is<core::type::Vector>()) {
                        replace(b.Call<spirv::ir::BuiltinCall>(
                            ty, spirv::BuiltinFn::kVectorTimesMatrix, lhs, rhs));
                    }
                }
                break;

            default:
                TINT_UNREACHABLE() << "unhandled matrix arithmetic instruction";
                break;
        }
    }

    // Replace the matrix convert instructions that we found.
    for (auto* convert : convert_worklist) {
        auto* arg = convert->Args()[core::ir::Convert::kValueOperandOffset];
        auto* in_mat = arg->Type()->As<core::type::Matrix>();
        auto* out_mat = convert->Result()->Type()->As<core::type::Matrix>();

        // Extract and convert each column separately.
        Vector<core::ir::Value*, 4> args;
        for (uint32_t c = 0; c < out_mat->columns(); c++) {
            b.InsertBefore(convert, [&] {
                auto* col = b.Access(in_mat->ColumnType(), arg, u32(c));
                auto* new_col = b.Convert(out_mat->ColumnType(), col);
                args.Push(new_col->Result());
            });
        }

        // Reconstruct the result matrix from the converted columns.
        auto* construct = b.Construct(out_mat, std::move(args));
        if (auto name = ir.NameOf(convert)) {
            ir.SetName(construct->Result(), name);
        }
        convert->Result()->ReplaceAllUsesWith(construct->Result());
        convert->ReplaceWith(construct);
        convert->Destroy();
    }
}

}  // namespace

Result<SuccessType> HandleMatrixArithmetic(core::ir::Module& ir) {
    auto result = ValidateAndDumpIfNeeded(ir, "HandleMatrixArithmetic transform");
    if (!result) {
        return result.Failure();
    }

    Run(ir);

    return Success;
}

}  // namespace tint::spirv::writer::raise
