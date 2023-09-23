#include "Resources.h"

#include "Logging.h"
namespace fs = std::filesystem;

namespace UniverseEngine {
    Resources::Resources() : scenes{}, textures{}, shaders{}, scenePaths{}, texturePaths{} {
    }

    std::shared_ptr<Scene> Resources::LoadScene(const std::filesystem::path& filePath) {
        auto scene = scenePaths.find(filePath);
        if (scene != scenePaths.end())
            return scene->second;

        std::shared_ptr<Scene> hScene;

        std::string extension = filePath.extension().string();
        if (extension == ".usd")  // TODO: Look into usdz
            hScene = LoadUSD(filePath);
        else if (extension == ".obj")
            hScene = LoadOBJ(filePath);
        else if (extension == ".gltf")
            hScene = LoadGLTF(filePath);
        else
            UE_FATAL("Cannot load unsupported scene type '%s'.", extension);

        scenePaths.insert(std::make_pair(filePath, hScene));
        return hScene;
    }

    std::shared_ptr<Texture> Resources::LoadTexture(const fs::path& filePath) {
        auto texture = texturePaths.find(filePath);
        if (texture != texturePaths.end())
            return texture->second;

        static const std::string supportedExtensions[8] = {".jpg", ".png", ".tga", ".bmp",
                                                           ".psd", ".gif", ".hdr", ".pic"};

        fs::path extension = filePath.extension();
        for (size_t i = 0; i < 8; i++) {
            if (supportedExtensions[i] == extension) {
                std::shared_ptr<Texture> hTexture = LoadIMG(filePath);
                texturePaths.insert(std::make_pair(filePath, hTexture));
                return hTexture;
            }
        }

        UE_FATAL("Cannot load unsupported texture type '%s'.", extension);
        return nullptr;
    }

    std::shared_ptr<Shader> Resources::LoadShader(const std::filesystem::path& filePath) {
        auto shader = this->shaderPaths.find(filePath);
        if (shader != this->shaderPaths.end())
            return shader->second;

        static const std::string supportedExtensions[2] = {".vert", ".frag"};

        fs::path extension = filePath.extension();
        for (size_t i = 0; i < 2; i++) {
            if (supportedExtensions[i] == extension) {
                std::shared_ptr<Shader> hShader = LoadShaderSource(filePath);
                this->shaderPaths.insert(std::make_pair(filePath, hShader));
                this->newShaders.push_back(hShader);
                return hShader;
            }
        }

        UE_FATAL("Cannot load unsupported shader type '%s'.", extension);
        return nullptr;
    }

   /* void Resources::DeleteScene(Handle<Scene> hScene) {
        this->scenes->Free(hScene);
    }

    void Resources::DeleteShader(Handle<Shader> hShader) {
        this->shaders->Free(hShader);

        for (auto& pair : this->shaderPaths) {
            if (pair.second == hShader) {
                this->shaderPaths.erase(pair.first);
                return;
            }
        }
    }*/

    const std::vector<std::shared_ptr<Shader>>& Resources::GetNewShaders() {
        return this->newShaders;
    }

    void Resources::Update() {
        this->newShaders.clear();
    }
}  // namespace UniverseEngine
