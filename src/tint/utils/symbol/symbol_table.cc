// Copyright 2020 The Dawn & Tint Authors
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

#include "tint/utils/symbol/symbol_table.h"

#include "tint/utils/ice/ice.h"

namespace tint {

SymbolTable::SymbolTable(tint::GenerationID generation_id) : generation_id_(generation_id) {}

SymbolTable::SymbolTable(SymbolTable&&) = default;

SymbolTable::~SymbolTable() = default;

SymbolTable& SymbolTable::operator=(SymbolTable&&) = default;

Symbol SymbolTable::Register(std::string_view name) {
    TINT_ASSERT(!name.empty());

    auto it = name_to_symbol_.Find(name);
    if (it) {
        return *it;
    }
    return RegisterInternal(name);
}

Symbol SymbolTable::RegisterInternal(std::string_view name) {
    char* name_mem = Bitcast<char*>(name_allocator_.Allocate(name.length() + 1));
    if (name_mem == nullptr) {
        TINT_ICE() << "failed to allocate memory for symbol's string";
        return Symbol();
    }

    memcpy(name_mem, name.data(), name.length() + 1);
    std::string_view nv(name_mem, name.length());

    Symbol sym(next_symbol_, generation_id_, nv);
    ++next_symbol_;
    name_to_symbol_.Add(sym.NameView(), sym);

    return sym;
}

Symbol SymbolTable::Get(std::string_view name) const {
    auto it = name_to_symbol_.Find(name);
    return it ? *it : Symbol();
}

Symbol SymbolTable::New(std::string_view prefix_view /* = "" */) {
    std::string prefix;
    if (prefix_view.empty()) {
        prefix = "tint_symbol";
    } else {
        prefix = std::string(prefix_view);
    }

    auto it = name_to_symbol_.Find(prefix);
    if (!it) {
        return RegisterInternal(prefix);
    }

    size_t i = 0;
    auto last_prefix = last_prefix_to_index_.Find(prefix);
    if (last_prefix) {
        i = *last_prefix;
    }

    std::string name;
    do {
        ++i;
        name = prefix + "_" + std::to_string(i);
    } while (name_to_symbol_.Contains(name));

    auto sym = RegisterInternal(name);
    if (last_prefix) {
        *last_prefix = i;
    } else {
        last_prefix_to_index_.Add(prefix, i);
    }
    return sym;
}

}  // namespace tint
