#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../ComputePipeline.h"
#include "../DescriptorSetLayout.h"
#include "../Format.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Mesh.h"
#include "../PushConstantRange.h"
#include "../RenderPass.h"
#include "../ShaderRenderable.h"
#include "VkConversion.h"

namespace UniverseEngine {
    ComputePipeline::ComputePipeline(
        std::shared_ptr<LogicalDevice> device, const ShaderRenderable& shader,
        std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts)
        : device(device), descriptorSetLayouts(descriptorSetLayouts) {
        VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
        computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        computeShaderStageInfo.module = shader.GetShader();
        computeShaderStageInfo.pName = "main";
        VkPipelineShaderStageCreateInfo shaderStages[] = {computeShaderStageInfo};

        std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts{};
        for (auto& descriptorSetLayout : descriptorSetLayouts) {
            vkDescriptorSetLayouts.push_back(descriptorSetLayout->GetLayout());
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(vkDescriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = vkDescriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        UE_ASSERT_MSG(!vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, nullptr,
                                              &this->pipelineLayout),
                      "Failed to create pipeline layout.");

        VkComputePipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineInfo.layout = this->pipelineLayout;
        pipelineInfo.stage = computeShaderStageInfo;

        UE_ASSERT_MSG(!vkCreateComputePipelines(device->GetDevice(), VK_NULL_HANDLE, 1,
                                                 &pipelineInfo, nullptr, &this->pipeline),
                      "Failed to create compute pipeline.");
    }

    ComputePipeline::~ComputePipeline() {
        vkDestroyPipeline(this->device->GetDevice(), this->pipeline, nullptr);
        vkDestroyPipelineLayout(this->device->GetDevice(), this->pipelineLayout, nullptr);
    }

    VkPipeline ComputePipeline::GetPipeline() const {
        return this->pipeline;
    }

    VkPipelineLayout ComputePipeline::GetLayout() const {
        return this->pipelineLayout;
    }
}  // namespace UniverseEngine
#endif