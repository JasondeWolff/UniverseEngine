#include "ImGuiRenderer.h"

#include <vector>

#include "Engine.h"

struct ImGuiPushConstant {
    glm::mat4 proj;
};

namespace UniverseEngine {
    glm::vec4 ImU32ToVec4(const ImU32& u32) {
        glm::vec4 result;
        result.r = static_cast<float>(u32 & 0xff) / 255.0f;
        result.g = static_cast<float>(u32 >> 8 & 0xff) / 255.0f;
        result.b = static_cast<float>(u32 >> 16 & 0xff) / 255.0f;
        result.a = static_cast<float>(u32 >> 24 & 0xff) / 255.0f;
        return result;
    }

    ImGuiRenderer::ImGuiRenderer(std::shared_ptr<LogicalDevice> device,
                                 const PhysicalDevice& physicalDevice,
                                 std::shared_ptr<DescriptorPool> descriptorPool,
                                 std::shared_ptr<RenderPass> renderPass, const Graphics& graphics) {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontDefault();
        io.Fonts->Build();

        int width, height;
        unsigned char* data;
        io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
        this->fontTexture =
            Engine::GetResources().CreateTexture("ImGui Font", data, static_cast<unsigned>(width),
                                                 static_cast<unsigned>(height), TextureType::SRGB);
        this->sampler = std::make_shared<Sampler>("Sampler", device, physicalDevice);

        this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            device, std::vector<DescriptorLayoutBinding>{
                        DescriptorLayoutBinding("tex", 0, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                GraphicsStageFlagBits::FRAGMENT_STAGE)});

        for (size_t i = 0; i < this->descriptorSets.size(); i++) {
            this->descriptorSets[i] =
                std::make_shared<DescriptorSet>(device, descriptorPool, this->descriptorSetLayout);
        }

        auto& resources = Engine::GetResources();
        std::shared_ptr<Shader> shaderVS = resources.LoadShader("Assets/Shaders/imgui.vert");
        std::shared_ptr<Shader> shaderFS = resources.LoadShader("Assets/Shaders/imgui.frag");
        graphics.RebuildShaders();
        GraphicsPipelineInfo info{};
        info.ignoreDepth = true;
        std::vector<const ShaderRenderable*> shaders = {&shaderVS->Renderable(),
                                                        &shaderFS->Renderable()};
        this->pipeline = std::make_shared<GraphicsPipeline>(
            device, shaders, renderPass,
            std::vector<std::shared_ptr<DescriptorSetLayout>>{this->descriptorSetLayout},
            std::vector<PushConstantRange>{PushConstantRange("pc", sizeof(ImGuiPushConstant),
                                                             GraphicsStageFlagBits::VERTEX_STAGE)},
            info);

        size_t vertexSize = sizeof(Vertex) * MAX_VERTICES;
        size_t indexSize = sizeof(uint32_t) * MAX_INDICES;

        this->vertexBuffer = std::make_shared<Buffer>(
            "ImGui Vertices", device, physicalDevice, BufferUsageBits::VERTEX_BUFFER,
            static_cast<uint64_t>(vertexSize), BufferLocation::CPU_TO_GPU);
        this->indexBuffer = std::make_shared<Buffer>(
            "ImGui Indices", device, physicalDevice, BufferUsageBits::INDEX_BUFFER,
            static_cast<uint64_t>(indexSize), BufferLocation::CPU_TO_GPU);
    }

    glm::mat4 OrthoMat(float left, float right, float bottom, float top, float near, float far) {
        float rml = right - left;
        float rpl = right + left;
        float tmb = top - bottom;
        float tpb = top + bottom;
        float fmn = far - near;
        return glm::mat4(glm::vec4(2.0 / rml, 0.0, 0.0, 0.0), glm::vec4(0.0, -2.0 / tmb, 0.0, 0.0),
                         glm::vec4(0.0, 0.0, -1.0 / fmn, 0.0),
                         glm::vec4(-(rpl / rml), -(tpb / tmb), -(near / fmn), 1.0));
    }

    void ImGuiRenderer::Render(CmdList& cmdList, size_t currentFrame) {
        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();

        cmdList.BindGraphicsPipeline(this->pipeline);

        this->descriptorSets[currentFrame]->SetImage(0, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     this->fontTexture->Renderable().GetImage(),
                                                     this->sampler);
        cmdList.BindDescriptorSet(this->descriptorSets[currentFrame], 0);

        Rect2D rect2D{};
        rect2D.extent.x =
            static_cast<unsigned>(drawData->DisplaySize.x * drawData->FramebufferScale.x);
        rect2D.extent.y =
            static_cast<unsigned>(drawData->DisplaySize.y * drawData->FramebufferScale.y);
        cmdList.SetViewport(rect2D);

        ImGuiPushConstant pushConstant{};
        pushConstant.proj = /*glm::ortho(
            static_cast<double>(drawData->DisplayPos.x),
            static_cast<double>(drawData->DisplayPos.x + drawData->DisplaySize.x),
            static_cast<double>(drawData->DisplayPos.y),
            static_cast<double>(drawData->DisplayPos.y + drawData->DisplaySize.y), -1.0, 1.0);*/
            OrthoMat(0.0, drawData->DisplaySize.x, 0.0, -drawData->DisplaySize.y, -1.0, 1.0);
        cmdList.PushConstant("pc", pushConstant, GraphicsStageFlagBits::VERTEX_STAGE);

        size_t vertexOffset = 0;
        size_t indexOffset = 0;
        std::vector<size_t> indexOffsets{};
        for (size_t i = 0; i < static_cast<size_t>(drawData->CmdListsCount); i++) {
            ImDrawList* drawList = drawData->CmdLists[static_cast<int>(i)];

            std::vector<Vertex> vertices;
            vertices.reserve(static_cast<size_t>(drawList->VtxBuffer.Size));
            for (auto& imVert : drawList->VtxBuffer) {
                Vertex vertex{};
                vertex.position = glm::vec3(imVert.pos.x, imVert.pos.y, 0.0f);
                vertex.texCoord = glm::vec2(imVert.uv.x, imVert.uv.y);
                vertex.color = ImU32ToVec4(imVert.col);
                vertices.push_back(vertex);
            }
            std::vector<uint32_t> indices;
            indices.reserve(static_cast<size_t>(drawList->IdxBuffer.Size));
            for (auto& imIdx : drawList->IdxBuffer) {
                indices.push_back(static_cast<uint32_t>(imIdx) +
                                  static_cast<uint32_t>(vertexOffset));
            }

            void* data = this->vertexBuffer->Map();
            memcpy(static_cast<Vertex*>(data) + vertexOffset, vertices.data(),
                   sizeof(Vertex) * vertices.size());
            this->vertexBuffer->Unmap();

            data = this->indexBuffer->Map();
            memcpy(static_cast<uint32_t*>(data) + indexOffset, indices.data(),
                   sizeof(uint32_t) * indices.size());
            this->indexBuffer->Unmap();

            indexOffsets.push_back(indexOffset);
            vertexOffset += vertices.size();
            indexOffset += indices.size();
        }

        cmdList.BindVertexBuffer(this->vertexBuffer);
        cmdList.BindIndexBuffer(this->indexBuffer);

        ImVec2& clipOffset = drawData->DisplayPos;
        ImVec2& clipScale = drawData->FramebufferScale;

        for (size_t i = 0; i < static_cast<size_t>(drawData->CmdListsCount); i++) {
            ImDrawList* drawList = drawData->CmdLists[static_cast<int>(i)];

            for (auto& imCmd : drawList->CmdBuffer) {
                ImVec4& clipRect = imCmd.ClipRect;

                float clipX = (clipRect.x - clipOffset.x) * clipScale.x;
                float clipY = (clipRect.y - clipOffset.y) * clipScale.y;
                float clipW = (clipRect.z - clipOffset.x) * clipScale.x - clipX;
                float clipH = (clipRect.w - clipOffset.y) * clipScale.y - clipY;

                Rect2D rect2D{};
                rect2D.offset.y = static_cast<unsigned>(glm::max(clipY, 0.0f));
                rect2D.offset.x = static_cast<unsigned>(glm::max(clipX, 0.0f));
                rect2D.extent.x = static_cast<unsigned>(clipW);
                rect2D.extent.y = static_cast<unsigned>(clipH);
                cmdList.SetScissor(rect2D);

                cmdList.DrawElements(static_cast<uint32_t>(imCmd.ElemCount), 1,
                                     static_cast<uint32_t>(indexOffsets[i]) + imCmd.IdxOffset);
            }
        }
    }
}  // namespace UniverseEngine