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

#ifndef SRC_TINT_LANG_GLSL_WRITER_AST_RAISE_TEXTURE_BUILTINS_FROM_UNIFORM_H_
#define SRC_TINT_LANG_GLSL_WRITER_AST_RAISE_TEXTURE_BUILTINS_FROM_UNIFORM_H_

#include <unordered_map>
#include <unordered_set>

#include "tint/api/common/binding_point.h"
#include "tint/api/options/texture_builtins_from_uniform.h"
#include "tint/lang/wgsl/ast/transform/transform.h"

// Forward declarations
namespace tint {
class CloneContext;
}  // namespace tint

namespace tint::glsl::writer {

/// TextureBuiltinsFromUniform is a transform that implements calls to textureNumLevels() and
/// textureNumSamples() by retrieving the texture information from a uniform buffer, as those
/// builtin functions are not available in some version of GLSL.
///
/// The generated uniform buffer will have the form:
/// ```
/// struct internal_uniform {
///  texture_builtin_value_0 : u32,
/// };
///
/// @group(0) @binding(0) var tex : texture_2d<f32>;
/// ```
/// The binding group and number used for this uniform buffer are provided via
/// the `Config` transform input.
///
/// The transform coverts the texture builtins calls into values lookup from the internal
/// buffer. If the texture is a function parameter instead of a global variable, this transform
/// also takes care of adding extra paramters and arguments to these functions and their callsites.
///
/// This transform must run before `CombineSamplers` transform so that the binding point of the
/// original texture object can be preserved.
class TextureBuiltinsFromUniform final
    : public Castable<TextureBuiltinsFromUniform, ast::transform::Transform> {
  public:
    /// Constructor
    TextureBuiltinsFromUniform();
    /// Destructor
    ~TextureBuiltinsFromUniform() override;

    /// Configuration options for the TextureBuiltinsFromUniform transform.
    struct Config final : public Castable<Config, ast::transform::Data> {
        /// Constructor
        /// @param ubo_bp the binding point to use for the generated uniform buffer.
        explicit Config(BindingPoint ubo_bp);

        /// Copy constructor
        Config(const Config&);

        /// Copy assignment
        /// @return this Config
        Config& operator=(const Config&);

        /// Destructor
        ~Config() override;

        /// The binding point to use for the generated uniform buffer.
        BindingPoint ubo_binding;
    };

    /// Information produced about what the transform did.
    /// If there were no calls to the textureNumLevels() or textureNumSamples() builtin, then no
    /// Result will be emitted.
    struct Result final : public Castable<Result, ast::transform::Data> {
        /// Using for shorter names
        /// Records the field and the byte offset of the data to push in the internal uniform
        /// buffer.
        using FieldAndOffset = TextureBuiltinsFromUniformOptions::FieldAndOffset;
        /// Maps from binding point to data entry with the information to populate the data.
        using BindingPointToFieldAndOffset =
            TextureBuiltinsFromUniformOptions::BindingPointToFieldAndOffset;

        /// Constructor
        /// @param bindpoint_to_data_in mapping from binding points of global texture variables to
        /// the byte offsets and data types needed to be pushed into the internal uniform buffer.
        explicit Result(BindingPointToFieldAndOffset bindpoint_to_data_in);

        /// Copy constructor
        Result(const Result&);

        /// Destructor
        ~Result() override;

        /// A map of global texture variable binding point to the byte offset and data type to push
        /// into the internal uniform buffer.
        BindingPointToFieldAndOffset bindpoint_to_data;
    };

    /// @copydoc ast::transform::Transform::Apply
    ApplyResult Apply(const Program& program,
                      const ast::transform::DataMap& inputs,
                      ast::transform::DataMap& outputs) const override;

  private:
    struct State;
};

}  // namespace tint::glsl::writer

#endif  // SRC_TINT_LANG_GLSL_WRITER_AST_RAISE_TEXTURE_BUILTINS_FROM_UNIFORM_H_
