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
        
    }

    TextureRenderable::~TextureRenderable() {
    }
}  // namespace UniverseEngine
#endif