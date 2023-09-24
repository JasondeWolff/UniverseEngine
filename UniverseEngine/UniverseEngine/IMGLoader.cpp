#include "Resources.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Logging.h"

namespace UniverseEngine {
    std::shared_ptr<Texture> Resources::LoadIMG(const std::filesystem::path& filePath) {
        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(filePath.string().c_str(), &width, &height, &nrChannels, 4);
        UE_ASSERT_MSG(data, "Failed to load image '%s'.", filePath.string().c_str());

        std::string name = filePath.filename().string();

        Texture parsedTexture(name, data, static_cast<unsigned>(width),
                              static_cast<unsigned>(height));

        return std::make_shared<Texture>(std::move(parsedTexture));
    }
}  // namespace UniverseEngine