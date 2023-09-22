#include "../DescriptorSetLayout.h"
#ifdef GRAPHICS_API_GL

#include "../DescriptorLayoutBinding.h"
#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    DescriptorSetLayout::DescriptorSetLayout(const std::shared_ptr<LogicalDevice> device,
                                             const std::vector<DescriptorLayoutBinding>& bindings)
        : device(device) {
    }

    DescriptorSetLayout::~DescriptorSetLayout() {
    }
}  // namespace UniverseEngine
#endif