#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Buffer.h"
#include "../CmdList.h"
#include "../DescriptorPool.h"
#include "../DescriptorSet.h"
#include "../DescriptorSetLayout.h"
#include "../Format.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Material.h"
#include "../MaterialRenderable.h"
#include "../PhysicalDevice.h"

namespace UniverseEngine {
    struct MaterialUniformBuffer {
        glm::vec4 baseColor;
        glm::vec4 emissiveFactor;
        float metallicFactor;
        float roughnessFactor;
        float normalFactor;
        float occlusionFactor;

        int hasBaseColorMap;
        int hasMetallicRoughnessMap;
        int hasEmissiveMap;
        int hasNormalMap;
        int hasOcclusionMap;

        float PADDING[3];
    };

    const std::shared_ptr<DescriptorSetLayout>& MaterialRenderable::DescriptorLayout(
        std::shared_ptr<LogicalDevice> device) {
        static std::shared_ptr<DescriptorSetLayout> descriptorLayout =
            std::make_shared<DescriptorSetLayout>(
                device,
                std::vector<DescriptorLayoutBinding>{
                    DescriptorLayoutBinding("baseColorMap", 2,
                                            DescriptorType::COMBINED_IMAGE_SAMPLER,
                                            GraphicsStageFlagBits::FRAGMENT_STAGE),
                    DescriptorLayoutBinding("metallicRoughnessMap", 3,
                                            DescriptorType::COMBINED_IMAGE_SAMPLER,
                                            GraphicsStageFlagBits::FRAGMENT_STAGE),
                    DescriptorLayoutBinding("emissiveMap", 4,
                                            DescriptorType::COMBINED_IMAGE_SAMPLER,
                                            GraphicsStageFlagBits::FRAGMENT_STAGE),
                    DescriptorLayoutBinding("normalMap", 5, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                            GraphicsStageFlagBits::FRAGMENT_STAGE),
                    DescriptorLayoutBinding("occlusionMap", 6,
                                            DescriptorType::COMBINED_IMAGE_SAMPLER,
                                            GraphicsStageFlagBits::FRAGMENT_STAGE),
                    DescriptorLayoutBinding("material", 7, DescriptorType::UNIFORM_BUFFER,
                                            GraphicsStageFlagBits::FRAGMENT_STAGE)});

        return descriptorLayout;
    }

    MaterialRenderable::MaterialRenderable(std::shared_ptr<LogicalDevice> device,
                                           const PhysicalDevice& physicalDevice,
                                           std::shared_ptr<DescriptorPool> descriptorPool,
                                           const Material& material)
        : device(device), physicalDevice(physicalDevice), material(material) {
        for (size_t i = 0; i < this->uniformBuffers.size(); i++) {
            this->uniformBuffers[i] =
                std::make_shared<Buffer>(Format("MaterialUniformBuffer_%i", i), device,
                                         physicalDevice, BufferUsageBits::UNIFORM_BUFFER,
                                         sizeof(MaterialUniformBuffer), BufferLocation::CPU_TO_GPU);

            this->descriptorSets[i] = std::make_shared<DescriptorSet>(
                device, descriptorPool, MaterialRenderable::DescriptorLayout(device));

            this->descriptorSets[i]->SetBuffer(7, DescriptorType::UNIFORM_BUFFER,
                                               this->uniformBuffers[i]);
        }
    }

    MaterialRenderable::~MaterialRenderable() {
    }

    void MaterialRenderable::Update(size_t currentFrame, std::shared_ptr<Sampler> sampler,
                                    CmdList& cmdList) {
        MaterialUniformBuffer uniformBuffer{};
        uniformBuffer.baseColor = this->material.baseColor;
        uniformBuffer.emissiveFactor = glm::vec4(this->material.emissiveFactor, 0.0);
        uniformBuffer.metallicFactor = this->material.metallicFactor;
        uniformBuffer.roughnessFactor = this->material.roughnessFactor;
        uniformBuffer.normalFactor = this->material.normalFactor;
        uniformBuffer.occlusionFactor = this->material.occlusionFactor;
        uniformBuffer.hasBaseColorMap = this->material.baseColorMap != nullptr;
        uniformBuffer.hasMetallicRoughnessMap = this->material.metallicRoughnessMap != nullptr;
        uniformBuffer.hasEmissiveMap = this->material.emissiveMap != nullptr;
        uniformBuffer.hasNormalMap = this->material.normalMap != nullptr;
        uniformBuffer.hasOcclusionMap = this->material.occlusionMap != nullptr;

        void* uniformBufferData = this->uniformBuffers[currentFrame]->Map();
        memcpy(uniformBufferData, &uniformBuffer, sizeof(MaterialUniformBuffer));
        this->uniformBuffers[currentFrame]->Unmap();

        if (this->material.baseColorMap) {
            this->descriptorSets[currentFrame]->SetImage(
                2, DescriptorType::COMBINED_IMAGE_SAMPLER,
                this->material.baseColorMap->renderable->GetImage(), sampler);
        } else {
            this->descriptorSets[currentFrame]->SetImage(
                2, DescriptorType::COMBINED_IMAGE_SAMPLER,
                TextureRenderable::EmptyImage(this->device, this->physicalDevice, cmdList),
                sampler);
        }

        if (this->material.metallicRoughnessMap) {
            this->descriptorSets[currentFrame]->SetImage(
                3, DescriptorType::COMBINED_IMAGE_SAMPLER,
                this->material.metallicRoughnessMap->renderable->GetImage(), sampler);
        } else {
            this->descriptorSets[currentFrame]->SetImage(
                3, DescriptorType::COMBINED_IMAGE_SAMPLER,
                TextureRenderable::EmptyImage(this->device, this->physicalDevice, cmdList),
                sampler);
        }

        if (this->material.emissiveMap) {
            this->descriptorSets[currentFrame]->SetImage(
                4, DescriptorType::COMBINED_IMAGE_SAMPLER,
                this->material.emissiveMap->renderable->GetImage(), sampler);
        } else {
            this->descriptorSets[currentFrame]->SetImage(
                4, DescriptorType::COMBINED_IMAGE_SAMPLER,
                TextureRenderable::EmptyImage(this->device, this->physicalDevice, cmdList),
                sampler);
        }

        if (this->material.normalMap) {
            this->descriptorSets[currentFrame]->SetImage(
                5, DescriptorType::COMBINED_IMAGE_SAMPLER,
                this->material.normalMap->renderable->GetImage(), sampler);
        } else {
            this->descriptorSets[currentFrame]->SetImage(
                5, DescriptorType::COMBINED_IMAGE_SAMPLER,
                TextureRenderable::EmptyImage(this->device, this->physicalDevice, cmdList),
                sampler);
        }

        if (this->material.occlusionMap) {
            this->descriptorSets[currentFrame]->SetImage(
                6, DescriptorType::COMBINED_IMAGE_SAMPLER,
                this->material.occlusionMap->renderable->GetImage(), sampler);
        } else {
            this->descriptorSets[currentFrame]->SetImage(
                6, DescriptorType::COMBINED_IMAGE_SAMPLER,
                TextureRenderable::EmptyImage(this->device, this->physicalDevice, cmdList),
                sampler);
        }
    }

    void MaterialRenderable::Bind(CmdList& cmdList, size_t currentFrame, uint32_t set) {
        //cmdList.BindDescriptorSet(this->descriptorSets[currentFrame], set);
        cmdList.BindDescriptorSet(this->descriptorSets[0], set);
    }
}  // namespace UniverseEngine
#endif