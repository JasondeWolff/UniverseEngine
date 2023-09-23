#include "../DescriptorSet.h"
#ifdef GRAPHICS_API_GL

#include "../Buffer.h"
#include "../DescriptorPool.h"
#include "../DescriptorSetLayout.h"
#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    DescriptorSet::DescriptorSet(const std::shared_ptr<LogicalDevice> device,
                                 const std::shared_ptr<DescriptorPool> pool,
                                 const std::shared_ptr<DescriptorSetLayout> layout)
        : device(device), pool(pool), layout(layout) {
    }

    DescriptorSet::~DescriptorSet() {
    }

    void DescriptorSet::SetBuffer(uint32_t set, uint32_t binding, DescriptorType type,
                                  const Buffer& buffer) {
    }
}  // namespace UniverseEngine
#endif