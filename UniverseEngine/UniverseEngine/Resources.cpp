#include "Resources.h"

namespace fs = std::filesystem;

namespace UniverseEngine {
	Resources::Resources() {
        this->scenes = std::make_unique<Pool<Scene>>();
		this->textures = std::make_unique<AtomicPool<Texture>>();
    }

    Handle<Scene> Resources::LoadScene(const std::filesystem::path& filePath) {
        std::string fileExtension = filePath.extension().string();
        if (fileExtension == ".usd") //fileExtension == ".usdz" (Need to look into)
            return LoadUSD(filePath);
        if (fileExtension == ".obj")
            return LoadOBJ(filePath);
        return Handle<Scene>::Invalid();
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

    void Resources::DeleteScene(Handle<Scene> hScene) {
        this->scenes->Free(hScene);
    }

    OptionalPtr<Scene> Resources::GetScene(Handle<Scene> hScene) {
        return this->scenes->Value(hScene);
    }

    OptionalPtr<Texture> Resources::GetTexture(AtomicHandle<Texture> hTexture) {
        return this->textures->Value(hTexture);
    }
}
