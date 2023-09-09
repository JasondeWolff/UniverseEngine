#include "Resources.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace UniverseEngine {
    AtomicHandle<Texture> Resources::LoadIMG(const std::filesystem::path& filePath) {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filePath.string().c_str(), &width, &height, &nrChannels, 4);

        std::string name = filePath.filename().string();

        Texture parsedTexture(name, data, static_cast<unsigned>(width),
                              static_cast<unsigned>(height));

        AtomicHandle<Texture> handle = this->textures->Alloc();
        this->textures->Value(handle).Value() = std::move(parsedTexture);
        return handle;
    }
}  // namespace UniverseEngine