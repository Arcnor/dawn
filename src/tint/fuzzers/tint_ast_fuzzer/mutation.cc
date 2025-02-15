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

#include "tint/fuzzers/tint_ast_fuzzer/mutation.h"

#include <cassert>

#include "tint/fuzzers/tint_ast_fuzzer/mutations/change_binary_operator.h"
#include "tint/fuzzers/tint_ast_fuzzer/mutations/change_unary_operator.h"
#include "tint/fuzzers/tint_ast_fuzzer/mutations/delete_statement.h"
#include "tint/fuzzers/tint_ast_fuzzer/mutations/replace_identifier.h"
#include "tint/fuzzers/tint_ast_fuzzer/mutations/wrap_unary_operator.h"

namespace tint::fuzzers::ast_fuzzer {

Mutation::~Mutation() = default;

std::unique_ptr<Mutation> Mutation::FromMessage(const protobufs::Mutation& message) {
    switch (message.mutation_case()) {
        case protobufs::Mutation::kChangeUnaryOperator:
            return std::make_unique<MutationChangeUnaryOperator>(message.change_unary_operator());
        case protobufs::Mutation::kReplaceIdentifier:
            return std::make_unique<MutationReplaceIdentifier>(message.replace_identifier());
        case protobufs::Mutation::kChangeBinaryOperator:
            return std::make_unique<MutationChangeBinaryOperator>(message.change_binary_operator());
        case protobufs::Mutation::kDeleteStatement:
            return std::make_unique<MutationDeleteStatement>(message.delete_statement());
        case protobufs::Mutation::kWrapUnaryOperator:
            return std::make_unique<MutationWrapUnaryOperator>(message.wrap_unary_operator());
        case protobufs::Mutation::MUTATION_NOT_SET:
            assert(false && "Mutation is not set");
            break;
    }
    return nullptr;
}

}  // namespace tint::fuzzers::ast_fuzzer
