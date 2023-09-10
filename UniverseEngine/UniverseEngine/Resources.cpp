#include "Resources.h"

namespace fs = std::filesystem;

namespace UniverseEngine {
	Resources::Resources() {
		this->models = std::make_unique<Pool<Model>>();
		this->textures = std::make_unique<AtomicPool<Texture>>();
    }

    Handle<Model> Resources::LoadModel(const fs::path& filePath) {
        std::string fileExtension = filePath.extension().string();
        if (fileExtension == ".usd")
            return LoadUSD(filePath);
        if (fileExtension == ".obj")
            return LoadOBJ(filePath);
        return Handle<Model>::Invalid();
    }

    AtomicHandle<Texture> Resources::LoadTexture(const fs::path& filePath) {
        static const std::string supportedExtensions[8] = {".jpg", ".png", ".tga", ".bmp",
                                                           ".psd", ".gif", ".hdr", ".pic"};

        fs::path extension = filePath.extension();
        for (size_t i = 0; i < 8; i++) {
            if (supportedExtensions[i] == extension) {
                return LoadIMG(filePath);
            }
        }

        UE_FATAL("Cannot load unsupported texture type '%s'.", extension);
        return AtomicHandle<Texture>::Invalid();
    }

    OptionalPtr<Model> Resources::GetModel(Handle<Model> handle) {
        return this->models->Value(handle);
    }

    OptionalPtr<Texture> Resources::GetTexture(AtomicHandle<Texture> handle) {
        return this->textures->Value(handle);
    }
}
