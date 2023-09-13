#include "Resources.h"

namespace fs = std::filesystem;

namespace UniverseEngine {
    Resources::Resources() : scenePaths{}, texturePaths{} {
        this->scenes = std::make_unique<Pool<Scene>>();
        this->textures = std::make_unique<AtomicPool<Texture>>();
    }

    Handle<Scene> Resources::LoadScene(const std::filesystem::path& filePath) {
        auto scene = scenePaths.find(filePath);
        if (scene != scenePaths.end())
            return scene->second;

        Handle<Scene> hScene;

        std::string extension = filePath.extension().string();
        if (extension == ".usd")
            hScene = LoadUSD(filePath);
        else if (extension == ".obj")
            hScene = LoadOBJ(filePath);
        else
            UE_FATAL("Cannot load unsupported scene type '%s'.", extension);

        scenePaths.insert(std::make_pair(filePath, hScene));
        return hScene;
    }

    AtomicHandle<Texture> Resources::LoadTexture(const fs::path& filePath) {
        auto texture = texturePaths.find(filePath);
        if (texture != texturePaths.end()) {
            auto& weak = texture->second;
            if (weak.IsAlive()) {
                return weak.Strong();
            } else {
                texturePaths.erase(texture);
            }
        }

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
}  // namespace UniverseEngine
