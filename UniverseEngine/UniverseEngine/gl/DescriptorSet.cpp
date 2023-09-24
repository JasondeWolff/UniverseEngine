#include "../DescriptorSet.h"
#ifdef GRAPHICS_API_GL

#include "../Buffer.h"
#include "../DescriptorPool.h"
#include "../DescriptorSetLayout.h"
#include "../Image.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Sampler.h"

namespace UniverseEngine {
    DescriptorSet::DescriptorSet(const std::shared_ptr<LogicalDevice> device,
                                 const std::shared_ptr<DescriptorPool> pool,
                                 const std::shared_ptr<DescriptorSetLayout> layout)
        : device(device), pool(pool), layout(layout) {
    }

    DescriptorSet::~DescriptorSet() {
    }

    void DescriptorSet::SetBuffer(uint32_t binding, DescriptorType type, const Buffer& buffer) {
    }

    void DescriptorSet::SetImage(uint32_t binding, DescriptorType type,
                                 std::shared_ptr<Image> image, std::shared_ptr<Sampler> sampler) {
        this->images[binding] = std::make_pair(image, sampler);
    }

    const std::unordered_map<uint32_t, std::pair<std::shared_ptr<Image>, std::shared_ptr<Sampler>>>&
    DescriptorSet::Images() const {
        return this->images;
    }
}  // namespace UniverseEngine
#endif