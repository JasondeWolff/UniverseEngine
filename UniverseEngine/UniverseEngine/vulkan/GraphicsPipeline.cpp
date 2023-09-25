#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../DescriptorSetLayout.h"
#include "../Format.h"
#include "../GraphicsPipeline.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Mesh.h"
#include "../PushConstantRange.h"
#include "../RenderPass.h"
#include "../ShaderRenderable.h"
#include "VkConversion.h"

namespace UniverseEngine {
    VkVertexInputBindingDescription VertexBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 4> VertexAttributeDescriptions(
        uint32_t binding = 0) {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        attributeDescriptions[0].binding = binding;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = binding;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        attributeDescriptions[2].binding = binding;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[3].binding = binding;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }

    GraphicsPipeline::GraphicsPipeline(std::shared_ptr<LogicalDevice> device,
                                       const std::vector<ShaderRenderable*>& shaders,
                                       std::shared_ptr<RenderPass> renderPass,
                                       std::shared_ptr<DescriptorSetLayout> descriptorSetLayout,
                                       std::vector<PushConstantRange> pushConstants)
        : device(device), renderPass(renderPass), descriptorSetLayout(descriptorSetLayout) {
        // TODO: make more dynamic
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = shaders[0]->GetShader();
        vertShaderStageInfo.pName = "main";
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = shaders[1]->GetShader();
        fragShaderStageInfo.pName = "main";
        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                                     VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        auto vertexBindingDescription = VertexBindingDescription();
        auto vertexAttributeDescriptions = VertexAttributeDescriptions();
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(vertexAttributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.lineWidth = 1.0;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        std::vector<VkPushConstantRange> vkPushConstantRanges;
        for (auto pushConstantRange : pushConstants) {
            VkPushConstantRange vkPushConstantRange{};
            vkPushConstantRange.size = static_cast<uint32_t>(pushConstantRange.size);
            vkPushConstantRange.stageFlags = GetVkShaderStageFlags(pushConstantRange.stageFlags);
            vkPushConstantRanges.push_back(vkPushConstantRange);
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        VkDescriptorSetLayout descriptorSetLayouts[] = {descriptorSetLayout->GetLayout()};
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts;
        pipelineLayoutInfo.pushConstantRangeCount =
            static_cast<uint32_t>(vkPushConstantRanges.size());
        pipelineLayoutInfo.pPushConstantRanges = vkPushConstantRanges.data();

        UE_ASSERT_MSG(!vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, nullptr,
                                              &this->pipelineLayout),
                      "Failed to create pipeline layout.");

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = 0.0f;
        viewport.height = 0.0f;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = {0, 0};

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;  // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = this->pipelineLayout;
        pipelineInfo.renderPass = renderPass->GetRenderPass();
        pipelineInfo.subpass = 0;

        UE_ASSERT_MSG(!vkCreateGraphicsPipelines(device->GetDevice(), VK_NULL_HANDLE, 1,
                                                 &pipelineInfo, nullptr, &this->pipeline),
                      "Failed to create graphics pipeline.");
    }

    GraphicsPipeline::~GraphicsPipeline() {
        vkDestroyPipeline(this->device->GetDevice(), this->pipeline, nullptr);
        vkDestroyPipelineLayout(this->device->GetDevice(), this->pipelineLayout, nullptr);
    }

    VkPipeline GraphicsPipeline::GetPipeline() const {
        return this->pipeline;
    }

    VkPipelineLayout GraphicsPipeline::GetLayout() const {
        return this->pipelineLayout;
    }
}  // namespace UniverseEngine
#endif