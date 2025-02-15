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

#include <string>
#include <vector>

#include "dawn/common/Assert.h"
#include "dawn/common/Constants.h"
#include "dawn/common/Math.h"
#include "dawn/tests/DawnTest.h"
#include "dawn/utils/ComboRenderPipelineDescriptor.h"
#include "dawn/utils/TestUtils.h"
#include "dawn/utils/TextureUtils.h"
#include "dawn/utils/WGPUHelpers.h"

namespace dawn {
namespace {

class StorageTextureTests : public DawnTest {
  public:
    static void FillExpectedData(void* pixelValuePtr,
                                 wgpu::TextureFormat format,
                                 uint32_t x,
                                 uint32_t y,
                                 uint32_t depthOrArrayLayer) {
        const uint32_t pixelValue = 1 + x + kWidth * (y + kHeight * depthOrArrayLayer);
        DAWN_ASSERT(pixelValue <= 255u / 4);

        switch (format) {
            // 32-bit unsigned integer formats
            case wgpu::TextureFormat::R32Uint: {
                uint32_t* valuePtr = static_cast<uint32_t*>(pixelValuePtr);
                *valuePtr = pixelValue;
                break;
            }

            case wgpu::TextureFormat::RG32Uint: {
                uint32_t* valuePtr = static_cast<uint32_t*>(pixelValuePtr);
                valuePtr[0] = pixelValue;
                valuePtr[1] = pixelValue * 2;
                break;
            }

            case wgpu::TextureFormat::RGBA32Uint: {
                uint32_t* valuePtr = static_cast<uint32_t*>(pixelValuePtr);
                valuePtr[0] = pixelValue;
                valuePtr[1] = pixelValue * 2;
                valuePtr[2] = pixelValue * 3;
                valuePtr[3] = pixelValue * 4;
                break;
            }

            // 32-bit signed integer formats
            case wgpu::TextureFormat::R32Sint: {
                int32_t* valuePtr = static_cast<int32_t*>(pixelValuePtr);
                *valuePtr = static_cast<int32_t>(pixelValue);
                break;
            }

            case wgpu::TextureFormat::RG32Sint: {
                int32_t* valuePtr = static_cast<int32_t*>(pixelValuePtr);
                valuePtr[0] = static_cast<int32_t>(pixelValue);
                valuePtr[1] = -static_cast<int32_t>(pixelValue);
                break;
            }

            case wgpu::TextureFormat::RGBA32Sint: {
                int32_t* valuePtr = static_cast<int32_t*>(pixelValuePtr);
                valuePtr[0] = static_cast<int32_t>(pixelValue);
                valuePtr[1] = -static_cast<int32_t>(pixelValue);
                valuePtr[2] = static_cast<int32_t>(pixelValue * 2);
                valuePtr[3] = -static_cast<int32_t>(pixelValue * 2);
                break;
            }

            // 32-bit float formats
            case wgpu::TextureFormat::R32Float: {
                float_t* valuePtr = static_cast<float_t*>(pixelValuePtr);
                *valuePtr = static_cast<float_t>(pixelValue * 1.1f);
                break;
            }

            case wgpu::TextureFormat::RG32Float: {
                float_t* valuePtr = static_cast<float_t*>(pixelValuePtr);
                valuePtr[0] = static_cast<float_t>(pixelValue * 1.1f);
                valuePtr[1] = -static_cast<float_t>(pixelValue * 2.2f);
                break;
            }

            case wgpu::TextureFormat::RGBA32Float: {
                float_t* valuePtr = static_cast<float_t*>(pixelValuePtr);
                valuePtr[0] = static_cast<float_t>(pixelValue * 1.1f);
                valuePtr[1] = -static_cast<float_t>(pixelValue * 1.1f);
                valuePtr[2] = static_cast<float_t>(pixelValue * 2.2f);
                valuePtr[3] = -static_cast<float_t>(pixelValue * 2.2f);
                break;
            }

            // 16-bit (unsigned integer, signed integer and float) 4-component formats
            case wgpu::TextureFormat::RGBA16Uint: {
                uint16_t* valuePtr = static_cast<uint16_t*>(pixelValuePtr);
                valuePtr[0] = static_cast<uint16_t>(pixelValue);
                valuePtr[1] = static_cast<uint16_t>(pixelValue * 2);
                valuePtr[2] = static_cast<uint16_t>(pixelValue * 3);
                valuePtr[3] = static_cast<uint16_t>(pixelValue * 4);
                break;
            }
            case wgpu::TextureFormat::RGBA16Sint: {
                int16_t* valuePtr = static_cast<int16_t*>(pixelValuePtr);
                valuePtr[0] = static_cast<int16_t>(pixelValue);
                valuePtr[1] = -static_cast<int16_t>(pixelValue);
                valuePtr[2] = static_cast<int16_t>(pixelValue * 2);
                valuePtr[3] = -static_cast<int16_t>(pixelValue * 2);
                break;
            }

            case wgpu::TextureFormat::RGBA16Float: {
                uint16_t* valuePtr = static_cast<uint16_t*>(pixelValuePtr);
                valuePtr[0] = Float32ToFloat16(static_cast<float_t>(pixelValue));
                valuePtr[1] = Float32ToFloat16(-static_cast<float_t>(pixelValue));
                valuePtr[2] = Float32ToFloat16(static_cast<float_t>(pixelValue * 2));
                valuePtr[3] = Float32ToFloat16(-static_cast<float_t>(pixelValue * 2));
                break;
            }

            // 8-bit (normalized/non-normalized signed/unsigned integer) 4-component formats
            case wgpu::TextureFormat::RGBA8Unorm:
            case wgpu::TextureFormat::RGBA8Uint: {
                utils::RGBA8* valuePtr = static_cast<utils::RGBA8*>(pixelValuePtr);
                *valuePtr =
                    utils::RGBA8(pixelValue, pixelValue * 2, pixelValue * 3, pixelValue * 4);
                break;
            }

            case wgpu::TextureFormat::BGRA8Unorm: {
                utils::RGBA8* valuePtr = static_cast<utils::RGBA8*>(pixelValuePtr);
                *valuePtr =
                    utils::RGBA8(pixelValue * 3, pixelValue * 2, pixelValue, pixelValue * 4);
                break;
            }

            case wgpu::TextureFormat::RGBA8Snorm:
            case wgpu::TextureFormat::RGBA8Sint: {
                int8_t* valuePtr = static_cast<int8_t*>(pixelValuePtr);
                valuePtr[0] = static_cast<int8_t>(pixelValue);
                valuePtr[1] = -static_cast<int8_t>(pixelValue);
                valuePtr[2] = static_cast<int8_t>(pixelValue) * 2;
                valuePtr[3] = -static_cast<int8_t>(pixelValue) * 2;
                break;
            }

            default:
                DAWN_UNREACHABLE();
                break;
        }
    }

    std::string GetImageDeclaration(wgpu::TextureFormat format,
                                    std::string accessQualifier,
                                    wgpu::TextureViewDimension dimension,
                                    uint32_t binding) {
        std::ostringstream ostream;
        ostream << "@group(0) @binding(" << binding << ") "
                << "var storageImage" << binding << " : ";
        switch (dimension) {
            case wgpu::TextureViewDimension::e1D:
                ostream << "texture_storage_1d";
                break;
            case wgpu::TextureViewDimension::e2D:
                ostream << "texture_storage_2d";
                break;
            case wgpu::TextureViewDimension::e2DArray:
                ostream << "texture_storage_2d_array";
                break;
            case wgpu::TextureViewDimension::e3D:
                ostream << "texture_storage_3d";
                break;
            default:
                DAWN_UNREACHABLE();
                break;
        }
        ostream << "<" << utils::GetWGSLImageFormatQualifier(format) << ", ";
        ostream << accessQualifier << ">;";
        return ostream.str();
    }

    const char* GetExpectedPixelValue(wgpu::TextureFormat format) {
        switch (format) {
            // non-normalized unsigned integer formats
            case wgpu::TextureFormat::R32Uint:
                return "vec4u(u32(value), 0u, 0u, 1u)";

            case wgpu::TextureFormat::RG32Uint:
                return "vec4u(u32(value), u32(value) * 2u, 0u, 1u)";

            case wgpu::TextureFormat::RGBA8Uint:
            case wgpu::TextureFormat::RGBA16Uint:
            case wgpu::TextureFormat::RGBA32Uint:
                return "vec4u(u32(value), u32(value) * 2u, "
                       "u32(value) * 3u, u32(value) * 4u)";

            // non-normalized signed integer formats
            case wgpu::TextureFormat::R32Sint:
                return "vec4i(i32(value), 0, 0, 1)";

            case wgpu::TextureFormat::RG32Sint:
                return "vec4i(i32(value), -i32(value), 0, 1)";

            case wgpu::TextureFormat::RGBA8Sint:
            case wgpu::TextureFormat::RGBA16Sint:
            case wgpu::TextureFormat::RGBA32Sint:
                return "vec4i(i32(value), -i32(value), i32(value) * 2, -i32(value) * 2)";

            // float formats
            case wgpu::TextureFormat::R32Float:
                return "vec4f(f32(value) * 1.1, 0.0, 0.0, 1.0)";

            case wgpu::TextureFormat::RG32Float:
                return "vec4f(f32(value) * 1.1, -f32(value) * 2.2, 0.0, 1.0)";

            case wgpu::TextureFormat::RGBA16Float:
                return "vec4f(f32(value), -f32(value), "
                       "f32(value) * 2.0, -f32(value) * 2.0)";

            case wgpu::TextureFormat::RGBA32Float:
                return "vec4f(f32(value) * 1.1, -f32(value) * 1.1, "
                       "f32(value) * 2.2, -f32(value) * 2.2)";

            // normalized signed/unsigned integer formats
            case wgpu::TextureFormat::RGBA8Unorm:
            case wgpu::TextureFormat::BGRA8Unorm:
                return "vec4f(f32(value) / 255.0, f32(value) / 255.0 * 2.0, "
                       "f32(value) / 255.0 * 3.0, f32(value) / 255.0 * 4.0)";

            case wgpu::TextureFormat::RGBA8Snorm:
                return "vec4f(f32(value) / 127.0, -f32(value) / 127.0, "
                       "f32(value) * 2.0 / 127.0, -f32(value) * 2.0 / 127.0)";

            default:
                DAWN_UNREACHABLE();
                break;
        }
    }

    const char* GetComparisonFunction(wgpu::TextureFormat format) {
        switch (format) {
            // non-normalized unsigned integer formats
            case wgpu::TextureFormat::R32Uint:
            case wgpu::TextureFormat::RG32Uint:
            case wgpu::TextureFormat::RGBA8Uint:
            case wgpu::TextureFormat::RGBA16Uint:
            case wgpu::TextureFormat::RGBA32Uint:
                return R"(
fn IsEqualTo(pixel : vec4u, expected : vec4u) -> bool {
  return all(pixel == expected);
})";

            // non-normalized signed integer formats
            case wgpu::TextureFormat::R32Sint:
            case wgpu::TextureFormat::RG32Sint:
            case wgpu::TextureFormat::RGBA8Sint:
            case wgpu::TextureFormat::RGBA16Sint:
            case wgpu::TextureFormat::RGBA32Sint:
                return R"(
fn IsEqualTo(pixel : vec4i, expected : vec4i) -> bool {
  return all(pixel == expected);
})";

            // float formats
            case wgpu::TextureFormat::R32Float:
            case wgpu::TextureFormat::RG32Float:
            case wgpu::TextureFormat::RGBA16Float:
            case wgpu::TextureFormat::RGBA32Float:
                return R"(
fn IsEqualTo(pixel : vec4f, expected : vec4f) -> bool {
  return all(pixel == expected);
})";

            // normalized signed/unsigned integer formats
            case wgpu::TextureFormat::RGBA8Unorm:
            case wgpu::TextureFormat::RGBA8Snorm:
                // On Windows Intel drivers the tests will fail if tolerance <= 0.00000001f.
                return R"(
fn IsEqualTo(pixel : vec4f, expected : vec4f) -> bool {
  let tolerance : f32 = 0.0000001;
  return all(abs(pixel - expected) < vec4f(tolerance, tolerance, tolerance, tolerance));
})";

            default:
                DAWN_UNREACHABLE();
                break;
        }

        return "";
    }

    std::string CommonWriteOnlyTestCode(
        const char* stage,
        wgpu::TextureFormat format,
        wgpu::TextureViewDimension dimension = wgpu::TextureViewDimension::e2D) {
        std::string componentFmt = utils::GetWGSLColorTextureComponentType(format);
        auto texelType = "vec4<" + componentFmt + ">";
        std::string sliceCount;
        std::string textureStore;
        std::string textureSize = "vec2i(textureDimensions(storageImage0).xy)";
        switch (dimension) {
            case wgpu::TextureViewDimension::e1D:
                sliceCount = "1";
                textureStore = "textureStore(storageImage0, x, expected)";
                textureSize = "vec2i(i32(textureDimensions(storageImage0)), 1)";
                break;
            case wgpu::TextureViewDimension::e2D:
                sliceCount = "1";
                textureStore = "textureStore(storageImage0, vec2i(x, y), expected)";
                break;
            case wgpu::TextureViewDimension::e2DArray:
                sliceCount = "i32(textureNumLayers(storageImage0))";
                textureStore = "textureStore(storageImage0, vec2i(x, y), slice, expected)";
                break;
            case wgpu::TextureViewDimension::e3D:
                sliceCount = "i32(textureDimensions(storageImage0).z)";
                textureStore = "textureStore(storageImage0, vec3i(x, y, slice), expected)";
                break;
            default:
                DAWN_UNREACHABLE();
                break;
        }
        const char* workgroupSize = !strcmp(stage, "compute") ? " @workgroup_size(1)" : "";
        const bool isFragment = strcmp(stage, "fragment") == 0;

        std::ostringstream ostream;
        ostream << GetImageDeclaration(format, "write", dimension, 0) << "\n";
        ostream << "@" << stage << workgroupSize << "\n";
        ostream << "fn main() ";
        if (isFragment) {
            ostream << "-> @location(0) vec4f ";
        }
        ostream << "{\n";
        ostream << "  let size : vec2i = " << textureSize << ";\n";
        ostream << "  let sliceCount : i32 = " << sliceCount << ";\n";
        ostream << "  for (var slice : i32 = 0; slice < sliceCount; slice = slice + 1) {\n";
        ostream << "    for (var y : i32 = 0; y < size.y; y = y + 1) {\n";
        ostream << "      for (var x : i32 = 0; x < size.x; x = x + 1) {\n";
        ostream << "        var value : i32 = " << kComputeExpectedValue << ";\n";
        ostream << "        var expected : " << texelType << " = " << GetExpectedPixelValue(format)
                << ";\n";
        ostream << "        " << textureStore << ";\n";
        ostream << "      }\n";
        ostream << "    }\n";
        ostream << "  }\n";
        if (isFragment) {
            ostream << "return vec4f();\n";
        }
        ostream << "}\n";

        return ostream.str();
    }

    static std::vector<uint8_t> GetExpectedData(wgpu::TextureFormat format,
                                                uint32_t sliceCount = 1) {
        const uint32_t texelSizeInBytes = utils::GetTexelBlockSizeInBytes(format);

        std::vector<uint8_t> outputData(texelSizeInBytes * kWidth * kHeight * sliceCount);

        for (uint32_t i = 0; i < outputData.size() / texelSizeInBytes; ++i) {
            uint8_t* pixelValuePtr = &outputData[i * texelSizeInBytes];
            const uint32_t x = i % kWidth;
            const uint32_t y = (i % (kWidth * kHeight)) / kWidth;
            const uint32_t slice = i / (kWidth * kHeight);
            FillExpectedData(pixelValuePtr, format, x, y, slice);
        }

        return outputData;
    }

    wgpu::Texture CreateTexture(wgpu::TextureFormat format,
                                wgpu::TextureUsage usage,
                                const wgpu::Extent3D& size,
                                wgpu::TextureDimension dimension = wgpu::TextureDimension::e2D) {
        wgpu::TextureDescriptor descriptor;
        descriptor.size = size;
        descriptor.dimension = dimension;
        descriptor.format = format;
        descriptor.usage = usage;
        return device.CreateTexture(&descriptor);
    }

    wgpu::Texture CreateTextureWithTestData(
        const uint8_t* initialTextureData,
        size_t initialTextureDataSize,
        wgpu::TextureFormat format,
        wgpu::TextureViewDimension dimension = wgpu::TextureViewDimension::e2D) {
        uint32_t texelSize = utils::GetTexelBlockSizeInBytes(format);
        DAWN_ASSERT(kWidth * texelSize <= kTextureBytesPerRowAlignment);

        const uint32_t bytesPerTextureRow = texelSize * kWidth;
        const uint32_t sliceCount =
            static_cast<uint32_t>(initialTextureDataSize / texelSize / (kWidth * kHeight));
        const size_t uploadBufferSize =
            kTextureBytesPerRowAlignment * (kHeight * sliceCount - 1) + kWidth * bytesPerTextureRow;

        std::vector<uint8_t> uploadBufferData(uploadBufferSize);
        for (uint32_t slice = 0; slice < sliceCount; ++slice) {
            const size_t initialDataOffset = bytesPerTextureRow * kHeight * slice;
            for (size_t y = 0; y < kHeight; ++y) {
                for (size_t x = 0; x < bytesPerTextureRow; ++x) {
                    uint8_t data =
                        initialTextureData[initialDataOffset + bytesPerTextureRow * y + x];
                    size_t indexInUploadBuffer =
                        (kHeight * slice + y) * kTextureBytesPerRowAlignment + x;
                    uploadBufferData[indexInUploadBuffer] = data;
                }
            }
        }
        wgpu::Buffer uploadBuffer =
            utils::CreateBufferFromData(device, uploadBufferData.data(), uploadBufferSize,
                                        wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::CopyDst);

        wgpu::Texture outputTexture = CreateTexture(
            format,
            wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc |
                wgpu::TextureUsage::CopyDst,
            {kWidth, kHeight, sliceCount}, utils::ViewDimensionToTextureDimension(dimension));

        wgpu::CommandEncoder encoder = device.CreateCommandEncoder();

        const wgpu::Extent3D copyExtent = {kWidth, kHeight, sliceCount};
        wgpu::ImageCopyBuffer imageCopyBuffer =
            utils::CreateImageCopyBuffer(uploadBuffer, 0, kTextureBytesPerRowAlignment, kHeight);
        wgpu::ImageCopyTexture imageCopyTexture;
        imageCopyTexture.texture = outputTexture;
        encoder.CopyBufferToTexture(&imageCopyBuffer, &imageCopyTexture, &copyExtent);

        wgpu::CommandBuffer commandBuffer = encoder.Finish();
        queue.Submit(1, &commandBuffer);

        return outputTexture;
    }

    wgpu::ComputePipeline CreateComputePipeline(const char* computeShader) {
        wgpu::ShaderModule csModule = utils::CreateShaderModule(device, computeShader);
        wgpu::ComputePipelineDescriptor computeDescriptor;
        computeDescriptor.layout = nullptr;
        computeDescriptor.compute.module = csModule;
        computeDescriptor.compute.entryPoint = "main";
        return device.CreateComputePipeline(&computeDescriptor);
    }

    wgpu::RenderPipeline CreateRenderPipeline(const char* vertexShader,
                                              const char* fragmentShader) {
        wgpu::ShaderModule vsModule = utils::CreateShaderModule(device, vertexShader);
        wgpu::ShaderModule fsModule = utils::CreateShaderModule(device, fragmentShader);

        utils::ComboRenderPipelineDescriptor desc;
        desc.vertex.module = vsModule;
        desc.cFragment.module = fsModule;
        desc.cTargets[0].format = kRenderAttachmentFormat;
        desc.primitive.topology = wgpu::PrimitiveTopology::PointList;
        return device.CreateRenderPipeline(&desc);
    }

    void CheckDrawsGreen(const char* vertexShader,
                         const char* fragmentShader,
                         wgpu::Texture readonlyStorageTexture) {
        wgpu::RenderPipeline pipeline = CreateRenderPipeline(vertexShader, fragmentShader);
        wgpu::BindGroup bindGroup = utils::MakeBindGroup(
            device, pipeline.GetBindGroupLayout(0), {{0, readonlyStorageTexture.CreateView()}});

        // Clear the render attachment to red at the beginning of the render pass.
        wgpu::Texture outputTexture = CreateTexture(
            kRenderAttachmentFormat,
            wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc, {1, 1});
        utils::ComboRenderPassDescriptor renderPassDescriptor({outputTexture.CreateView()});
        renderPassDescriptor.cColorAttachments[0].loadOp = wgpu::LoadOp::Clear;
        renderPassDescriptor.cColorAttachments[0].clearValue = {1.f, 0.f, 0.f, 1.f};
        wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
        wgpu::RenderPassEncoder renderPassEncoder = encoder.BeginRenderPass(&renderPassDescriptor);
        renderPassEncoder.SetBindGroup(0, bindGroup);
        renderPassEncoder.SetPipeline(pipeline);
        renderPassEncoder.Draw(1);
        renderPassEncoder.End();

        wgpu::CommandBuffer commandBuffer = encoder.Finish();
        queue.Submit(1, &commandBuffer);

        // Check if the contents in the output texture are all as expected (green).
        EXPECT_PIXEL_RGBA8_EQ(utils::RGBA8::kGreen, outputTexture, 0, 0)
            << "\nVertex Shader:\n"
            << vertexShader << "\n\nFragment Shader:\n"
            << fragmentShader;
    }

    void CheckResultInStorageBuffer(
        wgpu::Texture readonlyStorageTexture,
        const std::string& computeShader,
        wgpu::TextureViewDimension dimension = wgpu::TextureViewDimension::e2D) {
        wgpu::ComputePipeline pipeline = CreateComputePipeline(computeShader.c_str());

        // Clear the content of the result buffer into 0.
        constexpr uint32_t kInitialValue = 0;
        wgpu::Buffer resultBuffer =
            utils::CreateBufferFromData(device, &kInitialValue, sizeof(kInitialValue),
                                        wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopySrc);
        wgpu::TextureViewDescriptor descriptor;
        descriptor.dimension = dimension;
        wgpu::BindGroup bindGroup = utils::MakeBindGroup(
            device, pipeline.GetBindGroupLayout(0),
            {{0, readonlyStorageTexture.CreateView(&descriptor)}, {1, resultBuffer}});

        wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
        wgpu::ComputePassEncoder computeEncoder = encoder.BeginComputePass();
        computeEncoder.SetBindGroup(0, bindGroup);
        computeEncoder.SetPipeline(pipeline);
        computeEncoder.DispatchWorkgroups(1);
        computeEncoder.End();

        wgpu::CommandBuffer commandBuffer = encoder.Finish();
        queue.Submit(1, &commandBuffer);

        // Check if the contents in the result buffer are what we expect.
        constexpr uint32_t kExpectedValue = 1u;
        EXPECT_BUFFER_U32_RANGE_EQ(&kExpectedValue, resultBuffer, 0, 1u);
    }

    void WriteIntoStorageTextureInRenderPass(wgpu::Texture writeonlyStorageTexture,
                                             const char* vertexShader,
                                             const char* fragmentShader) {
        // Create a render pipeline that writes the expected pixel values into the storage texture
        // without fragment shader outputs.
        wgpu::RenderPipeline pipeline = CreateRenderPipeline(vertexShader, fragmentShader);
        wgpu::BindGroup bindGroup = utils::MakeBindGroup(
            device, pipeline.GetBindGroupLayout(0), {{0, writeonlyStorageTexture.CreateView()}});

        wgpu::CommandEncoder encoder = device.CreateCommandEncoder();

        wgpu::Texture placeholderOutputTexture = CreateTexture(
            kRenderAttachmentFormat,
            wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc, {1, 1});
        utils::ComboRenderPassDescriptor renderPassDescriptor(
            {placeholderOutputTexture.CreateView()});
        wgpu::RenderPassEncoder renderPassEncoder = encoder.BeginRenderPass(&renderPassDescriptor);
        renderPassEncoder.SetBindGroup(0, bindGroup);
        renderPassEncoder.SetPipeline(pipeline);
        renderPassEncoder.Draw(1);
        renderPassEncoder.End();
        wgpu::CommandBuffer commandBuffer = encoder.Finish();
        queue.Submit(1, &commandBuffer);
    }

    void WriteIntoStorageTextureInComputePass(
        wgpu::Texture writeonlyStorageTexture,
        const char* computeShader,
        wgpu::TextureViewDimension dimension = wgpu::TextureViewDimension::e2D) {
        // Create a compute pipeline that writes the expected pixel values into the storage texture.
        wgpu::TextureViewDescriptor descriptor;
        descriptor.dimension = dimension;
        wgpu::ComputePipeline pipeline = CreateComputePipeline(computeShader);
        wgpu::BindGroup bindGroup =
            utils::MakeBindGroup(device, pipeline.GetBindGroupLayout(0),
                                 {{0, writeonlyStorageTexture.CreateView(&descriptor)}});

        wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
        wgpu::ComputePassEncoder computePassEncoder = encoder.BeginComputePass();
        computePassEncoder.SetBindGroup(0, bindGroup);
        computePassEncoder.SetPipeline(pipeline);
        computePassEncoder.DispatchWorkgroups(1);
        computePassEncoder.End();
        wgpu::CommandBuffer commandBuffer = encoder.Finish();
        queue.Submit(1, &commandBuffer);
    }

    void ReadWriteIntoStorageTextureInComputePass(
        wgpu::Texture readonlyStorageTexture,
        wgpu::Texture writeonlyStorageTexture,
        const char* computeShader,
        wgpu::TextureViewDimension dimension = wgpu::TextureViewDimension::e2D) {
        // Create a compute pipeline that writes the expected pixel values into the storage texture.
        wgpu::TextureViewDescriptor descriptor;
        descriptor.dimension = dimension;
        wgpu::ComputePipeline pipeline = CreateComputePipeline(computeShader);
        wgpu::BindGroup bindGroup =
            utils::MakeBindGroup(device, pipeline.GetBindGroupLayout(0),
                                 {{0, writeonlyStorageTexture.CreateView(&descriptor)},
                                  {1, readonlyStorageTexture.CreateView(&descriptor)}});

        wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
        wgpu::ComputePassEncoder computePassEncoder = encoder.BeginComputePass();
        computePassEncoder.SetBindGroup(0, bindGroup);
        computePassEncoder.SetPipeline(pipeline);
        computePassEncoder.DispatchWorkgroups(1);
        computePassEncoder.End();
        wgpu::CommandBuffer commandBuffer = encoder.Finish();
        queue.Submit(1, &commandBuffer);
    }

    void CheckOutputStorageTexture(wgpu::Texture writeonlyStorageTexture,
                                   wgpu::TextureFormat format,
                                   const wgpu::Extent3D& size) {
        const std::vector<uint8_t>& expectedData = GetExpectedData(format, size.depthOrArrayLayers);
        CheckOutputStorageTexture(writeonlyStorageTexture, format, size, expectedData.data(),
                                  expectedData.size());
    }

    void CheckOutputStorageTexture(wgpu::Texture writeonlyStorageTexture,
                                   wgpu::TextureFormat format,
                                   const wgpu::Extent3D& size,
                                   const uint8_t* expectedData,
                                   size_t expectedDataSize) {
        // Copy the content from the write-only storage texture to the result buffer.
        wgpu::BufferDescriptor descriptor;
        descriptor.size =
            utils::RequiredBytesInCopy(kTextureBytesPerRowAlignment, size.height, size, format);
        descriptor.usage = wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::CopyDst;
        wgpu::Buffer resultBuffer = device.CreateBuffer(&descriptor);

        wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
        {
            wgpu::ImageCopyTexture imageCopyTexture =
                utils::CreateImageCopyTexture(writeonlyStorageTexture, 0, {0, 0, 0});
            wgpu::ImageCopyBuffer imageCopyBuffer = utils::CreateImageCopyBuffer(
                resultBuffer, 0, kTextureBytesPerRowAlignment, size.height);
            encoder.CopyTextureToBuffer(&imageCopyTexture, &imageCopyBuffer, &size);
        }
        wgpu::CommandBuffer commandBuffer = encoder.Finish();
        queue.Submit(1, &commandBuffer);

        // Check if the contents in the result buffer are what we expect.
        uint32_t texelSize = utils::GetTexelBlockSizeInBytes(format);
        DAWN_ASSERT(size.width * texelSize <= kTextureBytesPerRowAlignment);

        for (size_t z = 0; z < size.depthOrArrayLayers; ++z) {
            for (size_t y = 0; y < size.height; ++y) {
                const size_t resultBufferOffset =
                    kTextureBytesPerRowAlignment * (size.height * z + y);
                const size_t expectedDataOffset = texelSize * size.width * (size.height * z + y);
                EXPECT_BUFFER_U32_RANGE_EQ(
                    reinterpret_cast<const uint32_t*>(expectedData + expectedDataOffset),
                    resultBuffer, resultBufferOffset, texelSize);
            }
        }
    }

    static constexpr size_t kWidth = 4u;
    static constexpr size_t kHeight = 4u;
    static constexpr wgpu::TextureFormat kRenderAttachmentFormat = wgpu::TextureFormat::RGBA8Unorm;

    const char* kSimpleVertexShader = R"(
;
@vertex fn main() -> @builtin(position) vec4f {
  return vec4f(0.0, 0.0, 0.0, 1.0);
})";

    const char* kComputeExpectedValue = "1 + x + size.x * (y + size.y * slice)";
};

// Test that write-only storage textures are supported in compute shader.
TEST_P(StorageTextureTests, WriteonlyStorageTextureInComputeShader) {
    for (wgpu::TextureFormat format : utils::kAllTextureFormats) {
        if (!utils::TextureFormatSupportsStorageTexture(format, IsCompatibilityMode())) {
            continue;
        }

        // TODO(dawn:1877): Snorm copy failing ANGLE Swiftshader, need further investigation.
        if (format == wgpu::TextureFormat::RGBA8Snorm && IsANGLESwiftShader()) {
            continue;
        }

        // TODO(crbug.com/dawn/676): investigate why this test fails with RGBA8Snorm on Linux
        // Intel OpenGL and OpenGLES drivers.
        if (format == wgpu::TextureFormat::RGBA8Snorm && IsIntel() &&
            (IsOpenGL() || IsOpenGLES()) && IsLinux()) {
            continue;
        }

        // Prepare the write-only storage texture.
        wgpu::Texture writeonlyStorageTexture =
            CreateTexture(format, wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc,
                          {kWidth, kHeight});

        // Write the expected pixel values into the write-only storage texture.
        const std::string computeShader = CommonWriteOnlyTestCode("compute", format);
        WriteIntoStorageTextureInComputePass(writeonlyStorageTexture, computeShader.c_str());

        // Verify the pixel data in the write-only storage texture is expected.
        CheckOutputStorageTexture(writeonlyStorageTexture, format, {kWidth, kHeight});
    }
}

// Test that write-only storage textures are supported in fragment shader.
TEST_P(StorageTextureTests, WriteonlyStorageTextureInFragmentShader) {
    for (wgpu::TextureFormat format : utils::kAllTextureFormats) {
        if (!utils::TextureFormatSupportsStorageTexture(format, IsCompatibilityMode())) {
            continue;
        }

        // TODO(dawn:1877): Snorm copy failing ANGLE Swiftshader, need further investigation.
        if (format == wgpu::TextureFormat::RGBA8Snorm && IsANGLESwiftShader()) {
            continue;
        }

        // TODO(dawn:1503): ANGLE OpenGL fails blit emulation path when texture is not copied
        // explicitly via the mUseCopy = true workaround path.
        if (format == wgpu::TextureFormat::RGBA8Snorm && IsANGLE() && IsWindows()) {
            continue;
        }

        // TODO(crbug.com/dawn/676): investigate why this test fails with RGBA8Snorm on Linux
        // Intel OpenGL and OpenGLES drivers.
        if (format == wgpu::TextureFormat::RGBA8Snorm && IsIntel() &&
            (IsOpenGL() || IsOpenGLES()) && IsLinux()) {
            continue;
        }

        // Prepare the write-only storage texture.
        wgpu::Texture writeonlyStorageTexture =
            CreateTexture(format, wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc,
                          {kWidth, kHeight});

        // Write the expected pixel values into the write-only storage texture.
        const std::string fragmentShader = CommonWriteOnlyTestCode("fragment", format);
        WriteIntoStorageTextureInRenderPass(writeonlyStorageTexture, kSimpleVertexShader,
                                            fragmentShader.c_str());

        // Verify the pixel data in the write-only storage texture is expected.
        CheckOutputStorageTexture(writeonlyStorageTexture, format, {kWidth, kHeight});
    }
}

// Verify 2D array and 3D write-only storage textures work correctly.
TEST_P(StorageTextureTests, Writeonly2DArrayOr3DStorageTexture) {
    // TODO(crbug.com/dawn/547): implement 3D storage texture on OpenGL and OpenGLES.
    DAWN_TEST_UNSUPPORTED_IF(IsOpenGL() || IsOpenGLES());

    constexpr uint32_t kSliceCount = 3u;

    constexpr wgpu::TextureFormat kTextureFormat = wgpu::TextureFormat::R32Uint;

    wgpu::TextureViewDimension dimensions[] = {
        wgpu::TextureViewDimension::e2DArray,
        wgpu::TextureViewDimension::e3D,
    };

    // Prepare the write-only storage texture.
    for (wgpu::TextureViewDimension dimension : dimensions) {
        wgpu::Texture writeonlyStorageTexture = CreateTexture(
            kTextureFormat, wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc,
            {kWidth, kHeight, kSliceCount}, utils::ViewDimensionToTextureDimension(dimension));

        // Write the expected pixel values into the write-only storage texture.
        const std::string computeShader =
            CommonWriteOnlyTestCode("compute", kTextureFormat, dimension);
        WriteIntoStorageTextureInComputePass(writeonlyStorageTexture, computeShader.c_str(),
                                             dimension);

        // Verify the pixel data in the write-only storage texture is expected.
        CheckOutputStorageTexture(writeonlyStorageTexture, kTextureFormat,
                                  {kWidth, kHeight, kSliceCount});
    }
}

// Verify 1D write-only storage textures work correctly.
TEST_P(StorageTextureTests, Writeonly1DStorageTexture) {
    constexpr wgpu::TextureFormat kTextureFormat = wgpu::TextureFormat::R32Uint;

    // Prepare the write-only storage texture.
    wgpu::Texture writeonlyStorageTexture = CreateTexture(
        kTextureFormat, wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc,
        {kWidth, 1, 1}, wgpu::TextureDimension::e1D);

    // Write the expected pixel values into the write-only storage texture.
    const std::string computeShader =
        CommonWriteOnlyTestCode("compute", kTextureFormat, wgpu::TextureViewDimension::e1D);
    WriteIntoStorageTextureInComputePass(writeonlyStorageTexture, computeShader.c_str(),
                                         wgpu::TextureViewDimension::e1D);

    // Verify the pixel data in the write-only storage texture is expected.
    CheckOutputStorageTexture(writeonlyStorageTexture, kTextureFormat, {kWidth, 1, 1});
}

// Test that multiple dispatches to increment values by ping-ponging between a sampled texture and
// a write-only storage texture are synchronized in one pass.
TEST_P(StorageTextureTests, SampledAndWriteonlyStorageTexturePingPong) {
    constexpr wgpu::TextureFormat kTextureFormat = wgpu::TextureFormat::R32Uint;
    wgpu::Texture storageTexture1 =
        CreateTexture(kTextureFormat,
                      wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::StorageBinding |
                          wgpu::TextureUsage::CopySrc,
                      {1u, 1u});
    wgpu::Texture storageTexture2 = CreateTexture(
        kTextureFormat, wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::StorageBinding,
        {1u, 1u});
    wgpu::ShaderModule module = utils::CreateShaderModule(device, R"(
@group(0) @binding(0) var Src : texture_2d<u32>;
@group(0) @binding(1) var Dst : texture_storage_2d<r32uint, write>;
@compute @workgroup_size(1) fn main() {
  var srcValue : vec4u = textureLoad(Src, vec2i(0, 0), 0);
  srcValue.x = srcValue.x + 1u;
  textureStore(Dst, vec2i(0, 0), srcValue);
}
    )");

    wgpu::ComputePipelineDescriptor pipelineDesc = {};
    pipelineDesc.compute.module = module;
    pipelineDesc.compute.entryPoint = "main";
    wgpu::ComputePipeline pipeline = device.CreateComputePipeline(&pipelineDesc);

    // In bindGroupA storageTexture1 is bound as read-only storage texture and storageTexture2 is
    // bound as write-only storage texture.
    wgpu::BindGroup bindGroupA = utils::MakeBindGroup(device, pipeline.GetBindGroupLayout(0),
                                                      {
                                                          {0, storageTexture1.CreateView()},
                                                          {1, storageTexture2.CreateView()},
                                                      });

    // In bindGroupA storageTexture2 is bound as read-only storage texture and storageTexture1 is
    // bound as write-only storage texture.
    wgpu::BindGroup bindGroupB = utils::MakeBindGroup(device, pipeline.GetBindGroupLayout(0),
                                                      {
                                                          {0, storageTexture2.CreateView()},
                                                          {1, storageTexture1.CreateView()},
                                                      });

    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    wgpu::ComputePassEncoder pass = encoder.BeginComputePass();
    pass.SetPipeline(pipeline);

    // After the first dispatch the value in storageTexture2 should be 1u.
    pass.SetBindGroup(0, bindGroupA);
    pass.DispatchWorkgroups(1);

    // After the second dispatch the value in storageTexture1 should be 2u;
    pass.SetBindGroup(0, bindGroupB);
    pass.DispatchWorkgroups(1);

    pass.End();

    wgpu::BufferDescriptor bufferDescriptor;
    bufferDescriptor.size = sizeof(uint32_t);
    bufferDescriptor.usage = wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::CopyDst;
    wgpu::Buffer resultBuffer = device.CreateBuffer(&bufferDescriptor);

    wgpu::ImageCopyTexture imageCopyTexture;
    imageCopyTexture.texture = storageTexture1;

    wgpu::ImageCopyBuffer imageCopyBuffer = utils::CreateImageCopyBuffer(resultBuffer, 0, 256, 1);
    wgpu::Extent3D extent3D = {1, 1, 1};
    encoder.CopyTextureToBuffer(&imageCopyTexture, &imageCopyBuffer, &extent3D);

    wgpu::CommandBuffer commands = encoder.Finish();
    queue.Submit(1, &commands);

    constexpr uint32_t kFinalPixelValueInTexture1 = 2u;
    EXPECT_BUFFER_U32_EQ(kFinalPixelValueInTexture1, resultBuffer, 0);
}

DAWN_INSTANTIATE_TEST(StorageTextureTests,
                      D3D11Backend(),
                      D3D12Backend(),
                      MetalBackend(),
                      OpenGLBackend(),
                      OpenGLESBackend(),
                      VulkanBackend());

class BGRA8UnormStorageTextureTests : public StorageTextureTests {
  public:
    std::vector<wgpu::FeatureName> GetRequiredFeatures() override {
        if (SupportsFeatures({wgpu::FeatureName::BGRA8UnormStorage})) {
            mIsBGRA8UnormStorageSupported = true;
            return {wgpu::FeatureName::BGRA8UnormStorage};
        } else {
            mIsBGRA8UnormStorageSupported = false;
            return {};
        }
    }

    bool IsBGRA8UnormStorageSupported() { return mIsBGRA8UnormStorageSupported; }

  private:
    bool mIsBGRA8UnormStorageSupported = false;
};

// Test that BGRA8Unorm is supported to be used as storage texture in compute shaders when the
// optional feature 'bgra8unorm-storage' is supported.
TEST_P(BGRA8UnormStorageTextureTests, WriteonlyStorageTextureInComputeShader) {
    DAWN_TEST_UNSUPPORTED_IF(!IsBGRA8UnormStorageSupported());

    constexpr wgpu::TextureFormat kFormat = wgpu::TextureFormat::BGRA8Unorm;
    wgpu::Texture writeonlyStorageTexture =
        CreateTexture(kFormat, wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc,
                      {kWidth, kHeight});

    // Write the expected pixel values into the write-only storage texture.
    const std::string computeShader = CommonWriteOnlyTestCode("compute", kFormat);
    WriteIntoStorageTextureInComputePass(writeonlyStorageTexture, computeShader.c_str());

    // Verify the pixel data in the write-only storage texture is expected.
    CheckOutputStorageTexture(writeonlyStorageTexture, kFormat, {kWidth, kHeight});
}

// Test that BGRA8Unorm is supported to be used as storage texture in fragment shaders when the
// optional feature 'bgra8unorm-storage' is supported.
TEST_P(BGRA8UnormStorageTextureTests, WriteonlyStorageTextureInFragmentShader) {
    DAWN_TEST_UNSUPPORTED_IF(!IsBGRA8UnormStorageSupported());

    constexpr wgpu::TextureFormat kFormat = wgpu::TextureFormat::BGRA8Unorm;

    // Prepare the write-only storage texture.
    wgpu::Texture writeonlyStorageTexture =
        CreateTexture(kFormat, wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc,
                      {kWidth, kHeight});

    // Write the expected pixel values into the write-only storage texture.
    const std::string fragmentShader = CommonWriteOnlyTestCode("fragment", kFormat);
    WriteIntoStorageTextureInRenderPass(writeonlyStorageTexture, kSimpleVertexShader,
                                        fragmentShader.c_str());

    // Verify the pixel data in the write-only storage texture is expected.
    CheckOutputStorageTexture(writeonlyStorageTexture, kFormat, {kWidth, kHeight});
}

DAWN_INSTANTIATE_TEST(BGRA8UnormStorageTextureTests,
                      D3D11Backend(),
                      D3D12Backend(),
                      MetalBackend(),
                      OpenGLBackend(),
                      OpenGLESBackend(),
                      VulkanBackend());

class StorageTextureZeroInitTests : public StorageTextureTests {
  public:
    static std::vector<uint8_t> GetExpectedData() {
        constexpr wgpu::TextureFormat kTextureFormat = wgpu::TextureFormat::R32Uint;

        const uint32_t texelSizeInBytes = utils::GetTexelBlockSizeInBytes(kTextureFormat);
        const size_t kDataCount = texelSizeInBytes * kWidth * kHeight;
        std::vector<uint8_t> outputData(kDataCount, 0);

        uint32_t* outputDataPtr = reinterpret_cast<uint32_t*>(&outputData[0]);
        *outputDataPtr = 1u;

        return outputData;
    }

    const char* kCommonReadOnlyZeroInitTestCode = R"(
fn doTest() -> bool {
  for (var y : i32 = 0; y < 4; y = y + 1) {
    for (var x : i32 = 0; x < 4; x = x + 1) {
      var pixel : vec4u = textureLoad(srcImage, vec2i(x, y));
      if (any(pixel != vec4u(0u, 0u, 0u, 1u))) {
        return false;
      }
    }
  }
  return true;
})";

    const char* kCommonWriteOnlyZeroInitTestCodeFragment = R"(
@group(0) @binding(0) var dstImage : texture_storage_2d<r32uint, write>;

@fragment fn main() -> @location(0) vec4f {
  textureStore(dstImage, vec2i(0, 0), vec4u(1u, 0u, 0u, 1u));
  return vec4f();
})";
    const char* kCommonWriteOnlyZeroInitTestCodeCompute = R"(
@group(0) @binding(0) var dstImage : texture_storage_2d<r32uint, write>;

@compute @workgroup_size(1) fn main() {
  textureStore(dstImage, vec2i(0, 0), vec4u(1u, 0u, 0u, 1u));
})";
};

// Verify that the texture is correctly cleared to 0 before its first usage as a write-only storage
// storage texture in a render pass.
TEST_P(StorageTextureZeroInitTests, WriteonlyStorageTextureClearsToZeroInRenderPass) {
    // Prepare the write-only storage texture.
    wgpu::Texture writeonlyStorageTexture = CreateTexture(
        wgpu::TextureFormat::R32Uint,
        wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc, {kWidth, kHeight});

    WriteIntoStorageTextureInRenderPass(writeonlyStorageTexture, kSimpleVertexShader,
                                        kCommonWriteOnlyZeroInitTestCodeFragment);
    std::vector<uint8_t> expectedData = GetExpectedData();
    CheckOutputStorageTexture(writeonlyStorageTexture, wgpu::TextureFormat::R32Uint,
                              {kWidth, kHeight}, expectedData.data(), expectedData.size());
}

// Verify that the texture is correctly cleared to 0 before its first usage as a write-only storage
// texture in a compute pass.
TEST_P(StorageTextureZeroInitTests, WriteonlyStorageTextureClearsToZeroInComputePass) {
    // Prepare the write-only storage texture.
    wgpu::Texture writeonlyStorageTexture = CreateTexture(
        wgpu::TextureFormat::R32Uint,
        wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc, {kWidth, kHeight});

    WriteIntoStorageTextureInComputePass(writeonlyStorageTexture,
                                         kCommonWriteOnlyZeroInitTestCodeCompute);
    std::vector<uint8_t> expectedData = GetExpectedData();
    CheckOutputStorageTexture(writeonlyStorageTexture, wgpu::TextureFormat::R32Uint,
                              {kWidth, kHeight}, expectedData.data(), expectedData.size());
}

DAWN_INSTANTIATE_TEST(StorageTextureZeroInitTests,
                      D3D11Backend({"nonzero_clear_resources_on_creation_for_testing"}),
                      D3D12Backend({"nonzero_clear_resources_on_creation_for_testing"}),
                      OpenGLBackend({"nonzero_clear_resources_on_creation_for_testing"}),
                      OpenGLESBackend({"nonzero_clear_resources_on_creation_for_testing"}),
                      MetalBackend({"nonzero_clear_resources_on_creation_for_testing"}),
                      VulkanBackend({"nonzero_clear_resources_on_creation_for_testing"}));

class ReadWriteStorageTextureTests : public StorageTextureTests {
  public:
    std::vector<wgpu::FeatureName> GetRequiredFeatures() override {
        if (SupportsFeatures({wgpu::FeatureName::ChromiumExperimentalReadWriteStorageTexture})) {
            mIsReadWriteStorageTextureSupported = true;
            return {wgpu::FeatureName::ChromiumExperimentalReadWriteStorageTexture};
        } else {
            mIsReadWriteStorageTextureSupported = false;
            return {};
        }
    }

    bool IsReadWriteStorageTextureSupported() { return mIsReadWriteStorageTextureSupported; }

  private:
    bool mIsReadWriteStorageTextureSupported = false;
};

// Verify read-write storage texture can work correctly in compute shaders.
TEST_P(ReadWriteStorageTextureTests, ReadWriteStorageTextureInComputeShader) {
    DAWN_TEST_UNSUPPORTED_IF(!IsReadWriteStorageTextureSupported());

    std::array<uint32_t, kWidth * kHeight> inputData;
    std::array<uint32_t, kWidth * kHeight> expectedData;
    for (size_t i = 0; i < inputData.size(); ++i) {
        inputData[i] = i + 1;
        expectedData[i] = inputData[i] * 2;
    }

    wgpu::Texture readWriteStorageTexture = CreateTextureWithTestData(
        reinterpret_cast<const uint8_t*>(inputData.data()), inputData.size() * sizeof(uint32_t),
        wgpu::TextureFormat::R32Uint);

    std::ostringstream sstream;
    sstream << R"(
enable chromium_experimental_read_write_storage_texture;
@group(0) @binding(0) var rwImage : texture_storage_2d<r32uint, read_write>;

@compute @workgroup_size()"
            << kWidth << ", " << kHeight << R"()
fn main(@builtin(local_invocation_id) local_id: vec3<u32>,) {
  var data1 = textureLoad(rwImage, vec2i(local_id.xy));
  data1.x = data1.x * 2;
  textureStore(rwImage, vec2i(local_id.xy), data1);
})";

    wgpu::ComputePipeline pipeline = CreateComputePipeline(sstream.str().c_str());
    wgpu::BindGroup bindGroup = utils::MakeBindGroup(device, pipeline.GetBindGroupLayout(0),
                                                     {{0, readWriteStorageTexture.CreateView()}});

    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    wgpu::ComputePassEncoder computePassEncoder = encoder.BeginComputePass();
    computePassEncoder.SetBindGroup(0, bindGroup);
    computePassEncoder.SetPipeline(pipeline);
    computePassEncoder.DispatchWorkgroups(1);
    computePassEncoder.End();
    wgpu::CommandBuffer commandBuffer = encoder.Finish();
    queue.Submit(1, &commandBuffer);

    CheckOutputStorageTexture(readWriteStorageTexture, wgpu::TextureFormat::R32Uint,
                              {kWidth, kHeight},
                              reinterpret_cast<const uint8_t*>(expectedData.data()),
                              expectedData.size() * sizeof(uint32_t));
}

// Verify read-write storage texture can work correctly in fragment shaders.
TEST_P(ReadWriteStorageTextureTests, ReadWriteStorageTextureInFragmentShader) {
    DAWN_TEST_UNSUPPORTED_IF(!IsReadWriteStorageTextureSupported());

    std::array<uint32_t, kWidth * kHeight> inputData;
    std::array<uint32_t, kWidth * kHeight> expectedData;
    for (size_t i = 0; i < inputData.size(); ++i) {
        inputData[i] = i + 1;
        expectedData[i] = inputData[i] * 2;
    }

    wgpu::Texture readWriteStorageTexture = CreateTextureWithTestData(
        reinterpret_cast<const uint8_t*>(inputData.data()), inputData.size() * sizeof(uint32_t),
        wgpu::TextureFormat::R32Uint);

    wgpu::TextureDescriptor colorTextureDescriptor;
    colorTextureDescriptor.format = wgpu::TextureFormat::RGBA8Unorm;
    colorTextureDescriptor.size = {kWidth, kHeight, 1};
    colorTextureDescriptor.usage = wgpu::TextureUsage::RenderAttachment;
    wgpu::Texture dummyColorTexture = device.CreateTexture(&colorTextureDescriptor);

    wgpu::ShaderModule vsModule = utils::CreateShaderModule(device, R"(
 @vertex fn main(@builtin(vertex_index) VertexIndex : u32) -> @builtin(position) vec4f {
    var pos = array(
        vec2f(-2.0, -2.0),
        vec2f(-2.0,  2.0),
        vec2f( 2.0, -2.0),
        vec2f(-2.0,  2.0),
        vec2f( 2.0, -2.0),
        vec2f( 2.0,  2.0));
    return vec4f(pos[VertexIndex], 0.0, 1.0);
})");

    wgpu::ShaderModule fsModule = utils::CreateShaderModule(device, R"(
enable chromium_experimental_read_write_storage_texture;
@group(0) @binding(0) var rwImage : texture_storage_2d<r32uint, read_write>;
@fragment fn main(@builtin(position) fragcoord: vec4f) -> @location(0) vec4f {
    var data1 = textureLoad(rwImage, vec2i(fragcoord.xy));
    data1.x = data1.x * 2;
    textureStore(rwImage, vec2i(fragcoord.xy), data1);
    return vec4f(0.0, 1.0, 0.0, 1.0);
})");

    utils::ComboRenderPipelineDescriptor pipelineDescriptor;
    pipelineDescriptor.vertex.module = vsModule;
    pipelineDescriptor.cFragment.module = fsModule;
    pipelineDescriptor.cTargets[0].format = colorTextureDescriptor.format;
    wgpu::RenderPipeline renderPipeline = device.CreateRenderPipeline(&pipelineDescriptor);

    wgpu::BindGroup bindGroup = utils::MakeBindGroup(device, renderPipeline.GetBindGroupLayout(0),
                                                     {{0, readWriteStorageTexture.CreateView()}});

    utils::ComboRenderPassDescriptor renderPassDescriptor({dummyColorTexture.CreateView()});

    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    wgpu::RenderPassEncoder renderPassEncoder = encoder.BeginRenderPass(&renderPassDescriptor);
    renderPassEncoder.SetBindGroup(0, bindGroup);
    renderPassEncoder.SetPipeline(renderPipeline);
    renderPassEncoder.Draw(6);
    renderPassEncoder.End();
    wgpu::CommandBuffer commandBuffer = encoder.Finish();
    queue.Submit(1, &commandBuffer);

    CheckOutputStorageTexture(readWriteStorageTexture, wgpu::TextureFormat::R32Uint,
                              {kWidth, kHeight},
                              reinterpret_cast<const uint8_t*>(expectedData.data()),
                              expectedData.size() * sizeof(uint32_t));
}

// Verify read-only storage texture can work correctly in compute shaders.
TEST_P(ReadWriteStorageTextureTests, ReadOnlyStorageTextureInComputeShader) {
    DAWN_TEST_UNSUPPORTED_IF(!IsReadWriteStorageTextureSupported());

    constexpr wgpu::TextureFormat kStorageTextureFormat = wgpu::TextureFormat::R32Uint;
    const std::vector<uint8_t> kInitialTextureData = GetExpectedData(kStorageTextureFormat);
    wgpu::Texture readonlyStorageTexture = CreateTextureWithTestData(
        kInitialTextureData.data(), kInitialTextureData.size(), kStorageTextureFormat);

    std::ostringstream sstream;
    sstream << R"(
enable chromium_experimental_read_write_storage_texture;
@group(0) @binding(0) var srcImage : texture_storage_2d<r32uint, read>;
@group(0) @binding(1) var<storage, read_write> output : u32;

@compute @workgroup_size(1)
fn main() {
    for (var y = 0u; y < )"
            << kHeight << R"(; y++) {
        for (var x = 0u; x < )"
            << kWidth << R"(; x++) {
            var expected = vec4u(1u + x + y * 4u, 0, 0, 1u);
            var pixel = textureLoad(srcImage, vec2u(x, y));
            if (any(pixel != expected)) {
                output = 0u;
                return;
            }
        }
    }
    output = 1u;
})";
    uint32_t kInitialValue = 0xFF;
    wgpu::Buffer resultBuffer =
        utils::CreateBufferFromData(device, &kInitialValue, sizeof(kInitialValue),
                                    wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopySrc);

    wgpu::ComputePipeline pipeline = CreateComputePipeline(sstream.str().c_str());
    wgpu::BindGroup bindGroup =
        utils::MakeBindGroup(device, pipeline.GetBindGroupLayout(0),
                             {{0, readonlyStorageTexture.CreateView()}, {1, resultBuffer}});

    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    wgpu::ComputePassEncoder computePassEncoder = encoder.BeginComputePass();
    computePassEncoder.SetBindGroup(0, bindGroup);
    computePassEncoder.SetPipeline(pipeline);
    computePassEncoder.DispatchWorkgroups(1);
    computePassEncoder.End();
    wgpu::CommandBuffer commandBuffer = encoder.Finish();
    queue.Submit(1, &commandBuffer);

    // Check if the contents in the result buffer are what we expect.
    constexpr uint32_t kExpectedValue = 1u;
    EXPECT_BUFFER_U32_RANGE_EQ(&kExpectedValue, resultBuffer, 0, 1u);
}

// Verify read-only storage texture can work correctly in vertex shaders.
TEST_P(ReadWriteStorageTextureTests, ReadOnlyStorageTextureInVertexShader) {
    DAWN_TEST_UNSUPPORTED_IF(!IsReadWriteStorageTextureSupported());

    // TODO(dawn:1972): Implement read-only storage texture as sampled texture in vertex shader.
    DAWN_SUPPRESS_TEST_IF(IsOpenGLES());

    constexpr wgpu::TextureFormat kStorageTextureFormat = wgpu::TextureFormat::R32Uint;
    const std::vector<uint8_t> kInitialTextureData = GetExpectedData(kStorageTextureFormat);
    wgpu::Texture readonlyStorageTexture = CreateTextureWithTestData(
        kInitialTextureData.data(), kInitialTextureData.size(), kStorageTextureFormat);

    std::ostringstream vsstream;
    vsstream << R"(
enable chromium_experimental_read_write_storage_texture;
@group(0) @binding(0) var srcImage : texture_storage_2d<r32uint, read>;

struct VertexOutput {
    @builtin(position) pos: vec4f,
    @location(0) color: vec4f,
}

@vertex fn main() -> VertexOutput {
    var vertexOutput : VertexOutput;
    vertexOutput.pos = vec4f(0, 0, 0, 1);
    for (var y = 0u; y < )"
             << kHeight << R"(; y++) {
        for (var x = 0u; x < )"
             << kWidth << R"(; x++) {
            var expected = vec4u(1u + x + y * 4u, 0, 0, 1u);
            var pixel = textureLoad(srcImage, vec2u(x, y));
            if (any(pixel != expected)) {
                vertexOutput.color = vec4f(1, 0, 0, 1);
                return vertexOutput;
            }
        }
    }
    vertexOutput.color = vec4f(0, 1, 0, 1);
    return vertexOutput;
})";
    const char* kFragmentShader = R"(
struct FragmentInput {
    @location(0) color: vec4f,
}

@fragment fn main(fragmentInput : FragmentInput) -> @location(0) vec4f {
    return fragmentInput.color;
})";

    CheckDrawsGreen(vsstream.str().c_str(), kFragmentShader, readonlyStorageTexture);
}

// Verify read-only storage texture can work correctly in fragment shaders.
TEST_P(ReadWriteStorageTextureTests, ReadOnlyStorageTextureInFragmentShader) {
    DAWN_TEST_UNSUPPORTED_IF(!IsReadWriteStorageTextureSupported());

    constexpr wgpu::TextureFormat kStorageTextureFormat = wgpu::TextureFormat::R32Uint;
    const std::vector<uint8_t> kInitialTextureData = GetExpectedData(kStorageTextureFormat);
    wgpu::Texture readonlyStorageTexture = CreateTextureWithTestData(
        kInitialTextureData.data(), kInitialTextureData.size(), kStorageTextureFormat);

    std::ostringstream fsstream;
    fsstream << R"(
enable chromium_experimental_read_write_storage_texture;
@group(0) @binding(0) var srcImage : texture_storage_2d<r32uint, read>;

@fragment fn main() -> @location(0) vec4f {
    for (var y = 0u; y < )"
             << kHeight << R"(; y++) {
        for (var x = 0u; x < )"
             << kWidth << R"(; x++) {
            var expected = vec4u(1u + x + y * 4u, 0, 0, 1u);
            var pixel = textureLoad(srcImage, vec2u(x, y));
            if (any(pixel != expected)) {
                return vec4f(1, 0, 0, 1);
            }
        }
    }
    return vec4f(0, 1, 0, 1);
})";

    CheckDrawsGreen(kSimpleVertexShader, fsstream.str().c_str(), readonlyStorageTexture);
}

// Verify using read-write storage texture access in pipeline layout is compatible with write-only
// storage texture access in shader.
TEST_P(ReadWriteStorageTextureTests, ReadWriteInPipelineLayoutAndWriteOnlyInShader) {
    DAWN_TEST_UNSUPPORTED_IF(!IsReadWriteStorageTextureSupported());

    constexpr wgpu::TextureFormat kStorageTextureFormat = wgpu::TextureFormat::R32Uint;
    std::array<uint32_t, kWidth * kHeight> expectedData;
    for (size_t i = 0; i < expectedData.size(); ++i) {
        expectedData[i] = i + 1;
    }

    wgpu::Texture storageTexture = CreateTexture(
        wgpu::TextureFormat::R32Uint,
        wgpu::TextureUsage::StorageBinding | wgpu::TextureUsage::CopySrc, {kWidth, kHeight, 1});

    std::ostringstream sstream;
    sstream << R"(
enable chromium_experimental_read_write_storage_texture;
@group(0) @binding(0) var rwImage : texture_storage_2d<r32uint, write>;

@compute @workgroup_size()"
            << kWidth << ", " << kHeight << R"()
fn main(
  @builtin(local_invocation_id) local_id: vec3u,
  @builtin(local_invocation_index) local_index : u32) {
  let data1 = vec4u(local_index + 1u, 0, 0, 1);
  textureStore(rwImage, vec2i(local_id.xy), data1);
})";

    wgpu::BindGroupLayout bindGroupLayout = utils::MakeBindGroupLayout(
        device, {{0, wgpu::ShaderStage::Compute, wgpu::StorageTextureAccess::ReadWrite,
                  kStorageTextureFormat, wgpu::TextureViewDimension::e2D}});
    wgpu::ComputePipelineDescriptor computeDescriptor;
    computeDescriptor.layout = utils::MakePipelineLayout(device, {bindGroupLayout});
    computeDescriptor.compute.module = utils::CreateShaderModule(device, sstream.str().c_str());
    computeDescriptor.compute.entryPoint = "main";
    wgpu::ComputePipeline pipeline = device.CreateComputePipeline(&computeDescriptor);

    wgpu::BindGroup bindGroup =
        utils::MakeBindGroup(device, bindGroupLayout, {{0, storageTexture.CreateView()}});

    wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
    wgpu::ComputePassEncoder computePassEncoder = encoder.BeginComputePass();
    computePassEncoder.SetBindGroup(0, bindGroup);
    computePassEncoder.SetPipeline(pipeline);
    computePassEncoder.DispatchWorkgroups(1);
    computePassEncoder.End();
    wgpu::CommandBuffer commandBuffer = encoder.Finish();
    queue.Submit(1, &commandBuffer);

    CheckOutputStorageTexture(storageTexture, wgpu::TextureFormat::R32Uint, {kWidth, kHeight},
                              reinterpret_cast<const uint8_t*>(expectedData.data()),
                              expectedData.size() * sizeof(uint32_t));
}

DAWN_INSTANTIATE_TEST(ReadWriteStorageTextureTests,
                      D3D11Backend(),
                      D3D12Backend(),
                      OpenGLBackend(),
                      OpenGLESBackend(),
                      MetalBackend(),
                      VulkanBackend());

}  // anonymous namespace
}  // namespace dawn
