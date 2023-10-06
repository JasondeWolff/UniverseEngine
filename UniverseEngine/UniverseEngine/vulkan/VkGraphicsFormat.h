#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../GraphicsFormat.h"

namespace UniverseEngine {
    class VkGraphicsFormat {
    public:
        static VkFormat To(const GraphicsFormat& format) {
            switch (format) {
                case GraphicsFormat::R4G4_UNORM_PACK8:
                    return VkFormat::VK_FORMAT_R4G4_UNORM_PACK8;
                case GraphicsFormat::R4G4B4A4_UNORM_PACK16:
                    return VkFormat::VK_FORMAT_R4G4B4A4_UNORM_PACK16;
                case GraphicsFormat::B4G4R4A4_UNORM_PACK16:
                    return VkFormat::VK_FORMAT_B4G4R4A4_UNORM_PACK16;
                case GraphicsFormat::R5G6B5_UNORM_PACK16:
                    return VkFormat::VK_FORMAT_R5G6B5_UNORM_PACK16;
                case GraphicsFormat::B5G6R5_UNORM_PACK16:
                    return VkFormat::VK_FORMAT_B5G6R5_UNORM_PACK16;
                case GraphicsFormat::R5G5B5A1_UNORM_PACK16:
                    return VkFormat::VK_FORMAT_R5G5B5A1_UNORM_PACK16;
                case GraphicsFormat::B5G5R5A1_UNORM_PACK16:
                    return VkFormat::VK_FORMAT_B5G5R5A1_UNORM_PACK16;
                case GraphicsFormat::R8_UNORM:
                    return VkFormat::VK_FORMAT_R8_UNORM;
                case GraphicsFormat::R8_SNORM:
                    return VkFormat::VK_FORMAT_R8_SNORM;
                case GraphicsFormat::R8_USCALED:
                    return VkFormat::VK_FORMAT_R8_USCALED;
                case GraphicsFormat::R8_SSCALED:
                    return VkFormat::VK_FORMAT_R8_SSCALED;
                case GraphicsFormat::R8_UINT:
                    return VkFormat::VK_FORMAT_R8_UINT;
                case GraphicsFormat::R8_SINT:
                    return VkFormat::VK_FORMAT_R8_SINT;
                case GraphicsFormat::R8_SRGB:
                    return VkFormat::VK_FORMAT_R8_SRGB;
                case GraphicsFormat::R8G8_UNORM:
                    return VkFormat::VK_FORMAT_R8G8_UNORM;
                case GraphicsFormat::R8G8_SNORM:
                    return VkFormat::VK_FORMAT_R8G8_SNORM;
                case GraphicsFormat::R8G8_USCALED:
                    return VkFormat::VK_FORMAT_R8G8_USCALED;
                case GraphicsFormat::R8G8_SSCALED:
                    return VkFormat::VK_FORMAT_R8G8_SSCALED;
                case GraphicsFormat::R8G8_UINT:
                    return VkFormat::VK_FORMAT_R8G8_UINT;
                case GraphicsFormat::R8G8_SINT:
                    return VkFormat::VK_FORMAT_R8G8_SINT;
                case GraphicsFormat::R8G8_SRGB:
                    return VkFormat::VK_FORMAT_R8G8_SRGB;
                case GraphicsFormat::R8G8B8_UNORM:
                    return VkFormat::VK_FORMAT_R8G8B8_UNORM;
                case GraphicsFormat::R8G8B8_SNORM:
                    return VkFormat::VK_FORMAT_R8G8B8_SNORM;
                case GraphicsFormat::R8G8B8_USCALED:
                    return VkFormat::VK_FORMAT_R8G8B8_USCALED;
                case GraphicsFormat::R8G8B8_SSCALED:
                    return VkFormat::VK_FORMAT_R8G8B8_SSCALED;
                case GraphicsFormat::R8G8B8_UINT:
                    return VkFormat::VK_FORMAT_R8G8B8_UINT;
                case GraphicsFormat::R8G8B8_SINT:
                    return VkFormat::VK_FORMAT_R8G8B8_SINT;
                case GraphicsFormat::R8G8B8_SRGB:
                    return VkFormat::VK_FORMAT_R8G8B8_SRGB;
                case GraphicsFormat::B8G8R8_UNORM:
                    return VkFormat::VK_FORMAT_B8G8R8_UNORM;
                case GraphicsFormat::B8G8R8_SNORM:
                    return VkFormat::VK_FORMAT_B8G8R8_SNORM;
                case GraphicsFormat::B8G8R8_USCALED:
                    return VkFormat::VK_FORMAT_B8G8R8_USCALED;
                case GraphicsFormat::B8G8R8_SSCALED:
                    return VkFormat::VK_FORMAT_B8G8R8_SSCALED;
                case GraphicsFormat::B8G8R8_UINT:
                    return VkFormat::VK_FORMAT_B8G8R8_UINT;
                case GraphicsFormat::B8G8R8_SINT:
                    return VkFormat::VK_FORMAT_B8G8R8_SINT;
                case GraphicsFormat::B8G8R8_SRGB:
                    return VkFormat::VK_FORMAT_B8G8R8_SRGB;
                case GraphicsFormat::R8G8B8A8_UNORM:
                    return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
                case GraphicsFormat::R8G8B8A8_SNORM:
                    return VkFormat::VK_FORMAT_R8G8B8A8_SNORM;
                case GraphicsFormat::R8G8B8A8_USCALED:
                    return VkFormat::VK_FORMAT_R8G8B8A8_USCALED;
                case GraphicsFormat::R8G8B8A8_SSCALED:
                    return VkFormat::VK_FORMAT_R8G8B8A8_SSCALED;
                case GraphicsFormat::R8G8B8A8_UINT:
                    return VkFormat::VK_FORMAT_R8G8B8A8_UINT;
                case GraphicsFormat::R8G8B8A8_SINT:
                    return VkFormat::VK_FORMAT_R8G8B8A8_SINT;
                case GraphicsFormat::R8G8B8A8_SRGB:
                    return VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
                case GraphicsFormat::B8G8R8A8_UNORM:
                    return VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
                case GraphicsFormat::B8G8R8A8_SNORM:
                    return VkFormat::VK_FORMAT_B8G8R8A8_SNORM;
                case GraphicsFormat::B8G8R8A8_USCALED:
                    return VkFormat::VK_FORMAT_B8G8R8A8_USCALED;
                case GraphicsFormat::B8G8R8A8_SSCALED:
                    return VkFormat::VK_FORMAT_B8G8R8A8_SSCALED;
                case GraphicsFormat::B8G8R8A8_UINT:
                    return VkFormat::VK_FORMAT_B8G8R8A8_UINT;
                case GraphicsFormat::B8G8R8A8_SINT:
                    return VkFormat::VK_FORMAT_B8G8R8A8_SINT;
                case GraphicsFormat::B8G8R8A8_SRGB:
                    return VkFormat::VK_FORMAT_B8G8R8A8_SRGB;
                case GraphicsFormat::D32_SFLOAT:
                    return VkFormat::VK_FORMAT_D32_SFLOAT;
                case GraphicsFormat::D24_UNORM_S8_UINT:
                    return VkFormat::VK_FORMAT_D24_UNORM_S8_UINT;
                case GraphicsFormat::R32_SFLOAT:
                    return VkFormat::VK_FORMAT_R32_SFLOAT;
            }
            return VkFormat::VK_FORMAT_UNDEFINED;
        }

        static GraphicsFormat From(const VkFormat& vkFormat) {
            switch (vkFormat) {
                case VkFormat::VK_FORMAT_R4G4_UNORM_PACK8:
                    return GraphicsFormat::R4G4_UNORM_PACK8;
                case VkFormat::VK_FORMAT_R4G4B4A4_UNORM_PACK16:
                    return GraphicsFormat::R4G4B4A4_UNORM_PACK16;
                case VkFormat::VK_FORMAT_B4G4R4A4_UNORM_PACK16:
                    return GraphicsFormat::B4G4R4A4_UNORM_PACK16;
                case VkFormat::VK_FORMAT_R5G6B5_UNORM_PACK16:
                    return GraphicsFormat::R5G6B5_UNORM_PACK16;
                case VkFormat::VK_FORMAT_B5G6R5_UNORM_PACK16:
                    return GraphicsFormat::B5G6R5_UNORM_PACK16;
                case VkFormat::VK_FORMAT_R5G5B5A1_UNORM_PACK16:
                    return GraphicsFormat::R5G5B5A1_UNORM_PACK16;
                case VkFormat::VK_FORMAT_B5G5R5A1_UNORM_PACK16:
                    return GraphicsFormat::B5G5R5A1_UNORM_PACK16;
                case VkFormat::VK_FORMAT_R8_UNORM:
                    return GraphicsFormat::R8_UNORM;
                case VkFormat::VK_FORMAT_R8_SNORM:
                    return GraphicsFormat::R8_SNORM;
                case VkFormat::VK_FORMAT_R8_USCALED:
                    return GraphicsFormat::R8_USCALED;
                case VkFormat::VK_FORMAT_R8_SSCALED:
                    return GraphicsFormat::R8_SSCALED;
                case VkFormat::VK_FORMAT_R8_UINT:
                    return GraphicsFormat::R8_UINT;
                case VkFormat::VK_FORMAT_R8_SINT:
                    return GraphicsFormat::R8_SINT;
                case VkFormat::VK_FORMAT_R8_SRGB:
                    return GraphicsFormat::R8_SRGB;
                case VkFormat::VK_FORMAT_R8G8_UNORM:
                    return GraphicsFormat::R8G8_UNORM;
                case VkFormat::VK_FORMAT_R8G8_SNORM:
                    return GraphicsFormat::R8G8_SNORM;
                case VkFormat::VK_FORMAT_R8G8_USCALED:
                    return GraphicsFormat::R8G8_USCALED;
                case VkFormat::VK_FORMAT_R8G8_SSCALED:
                    return GraphicsFormat::R8G8_SSCALED;
                case VkFormat::VK_FORMAT_R8G8_UINT:
                    return GraphicsFormat::R8G8_UINT;
                case VkFormat::VK_FORMAT_R8G8_SINT:
                    return GraphicsFormat::R8G8_SINT;
                case VkFormat::VK_FORMAT_R8G8_SRGB:
                    return GraphicsFormat::R8G8_SRGB;
                case VkFormat::VK_FORMAT_R8G8B8_UNORM:
                    return GraphicsFormat::R8G8B8_UNORM;
                case VkFormat::VK_FORMAT_R8G8B8_SNORM:
                    return GraphicsFormat::R8G8B8_SNORM;
                case VkFormat::VK_FORMAT_R8G8B8_USCALED:
                    return GraphicsFormat::R8G8B8_USCALED;
                case VkFormat::VK_FORMAT_R8G8B8_SSCALED:
                    return GraphicsFormat::R8G8B8_SSCALED;
                case VkFormat::VK_FORMAT_R8G8B8_UINT:
                    return GraphicsFormat::R8G8B8_UINT;
                case VkFormat::VK_FORMAT_R8G8B8_SINT:
                    return GraphicsFormat::R8G8B8_SINT;
                case VkFormat::VK_FORMAT_R8G8B8_SRGB:
                    return GraphicsFormat::R8G8B8_SRGB;
                case VkFormat::VK_FORMAT_B8G8R8_UNORM:
                    return GraphicsFormat::B8G8R8_UNORM;
                case VkFormat::VK_FORMAT_B8G8R8_SNORM:
                    return GraphicsFormat::B8G8R8_SNORM;
                case VkFormat::VK_FORMAT_B8G8R8_USCALED:
                    return GraphicsFormat::B8G8R8_USCALED;
                case VkFormat::VK_FORMAT_B8G8R8_SSCALED:
                    return GraphicsFormat::B8G8R8_SSCALED;
                case VkFormat::VK_FORMAT_B8G8R8_UINT:
                    return GraphicsFormat::B8G8R8_UINT;
                case VkFormat::VK_FORMAT_B8G8R8_SINT:
                    return GraphicsFormat::B8G8R8_SINT;
                case VkFormat::VK_FORMAT_B8G8R8_SRGB:
                    return GraphicsFormat::B8G8R8_SRGB;
                case VkFormat::VK_FORMAT_R8G8B8A8_UNORM:
                    return GraphicsFormat::R8G8B8A8_UNORM;
                case VkFormat::VK_FORMAT_R8G8B8A8_SNORM:
                    return GraphicsFormat::R8G8B8A8_SNORM;
                case VkFormat::VK_FORMAT_R8G8B8A8_USCALED:
                    return GraphicsFormat::R8G8B8A8_USCALED;
                case VkFormat::VK_FORMAT_R8G8B8A8_SSCALED:
                    return GraphicsFormat::R8G8B8A8_SSCALED;
                case VkFormat::VK_FORMAT_R8G8B8A8_UINT:
                    return GraphicsFormat::R8G8B8A8_UINT;
                case VkFormat::VK_FORMAT_R8G8B8A8_SINT:
                    return GraphicsFormat::R8G8B8A8_SINT;
                case VkFormat::VK_FORMAT_R8G8B8A8_SRGB:
                    return GraphicsFormat::R8G8B8A8_SRGB;
                case VkFormat::VK_FORMAT_B8G8R8A8_UNORM:
                    return GraphicsFormat::B8G8R8A8_UNORM;
                case VkFormat::VK_FORMAT_B8G8R8A8_SNORM:
                    return GraphicsFormat::B8G8R8A8_SNORM;
                case VkFormat::VK_FORMAT_B8G8R8A8_USCALED:
                    return GraphicsFormat::B8G8R8A8_USCALED;
                case VkFormat::VK_FORMAT_B8G8R8A8_SSCALED:
                    return GraphicsFormat::B8G8R8A8_SSCALED;
                case VkFormat::VK_FORMAT_B8G8R8A8_UINT:
                    return GraphicsFormat::B8G8R8A8_UINT;
                case VkFormat::VK_FORMAT_B8G8R8A8_SINT:
                    return GraphicsFormat::B8G8R8A8_SINT;
                case VkFormat::VK_FORMAT_B8G8R8A8_SRGB:
                    return GraphicsFormat::B8G8R8A8_SRGB;
                case VkFormat::VK_FORMAT_D32_SFLOAT:
                    return GraphicsFormat::D32_SFLOAT;
                case VkFormat::VK_FORMAT_D24_UNORM_S8_UINT:
                    return GraphicsFormat::D24_UNORM_S8_UINT;
                case VkFormat::VK_FORMAT_R32_SFLOAT:
                    return GraphicsFormat::R32_SFLOAT;
            }
            return GraphicsFormat::UNDEFINED;
        }
    };
}
#endif