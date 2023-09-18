#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "VkDebugNames.h"
#include "VkPFN.h"

namespace UniverseEngine {
    void VkDebugNames::Set(const LogicalDevice& device, VkObjectType type, uint64_t handle, const std::string& name) {
        VkDebugUtilsObjectNameInfoEXT nameInfo{};
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.objectType = type;
        nameInfo.objectHandle = handle;
        nameInfo.pObjectName = name.c_str();
        
        VkPFN::vkSetDebugUtilsObjectNameEXT(device.GetDevice(), &nameInfo);
    }
}  // namespace UniverseEngine
#endif