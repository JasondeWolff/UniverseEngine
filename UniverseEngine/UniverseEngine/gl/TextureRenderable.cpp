#include "../TextureRenderable.h"
#ifdef GRAPHICS_API_GL

#include "../CmdList.h"
#include "../Format.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../PhysicalDevice.h"
#include "../Texture.h"

namespace UniverseEngine {
    TextureRenderable::TextureRenderable(std::shared_ptr<LogicalDevice> device,
                                         const PhysicalDevice& physicalDevice,
                                         CmdList& uploadCmdList, const Texture& texture) {
        this->image = std::make_shared<Image>(texture.data, texture.width, texture.height,
                                              GraphicsFormat::R8G8B8A8_SRGB);
    }

    TextureRenderable::~TextureRenderable() {
    }
}  // namespace UniverseEngine
#endif