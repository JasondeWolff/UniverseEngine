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

        this->scenes.push_back(hScene);
        this->scenePaths.insert(std::make_pair(filePath, hScene));
        return hScene;
    }

    std::shared_ptr<Scene> Resources::CreateScene(Mesh&& mesh) {
        std::shared_ptr<Scene> hScene;

        hScene.get()->meshes.push_back(std::move(mesh));
        //Add materials

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
                this->texturePaths.insert(std::make_pair(filePath, hTexture));
                this->newTextures.push_back(hTexture);
                this->textures.push_back(hTexture);
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
                this->shaders.push_back(hShader);
                return hShader;
            }
        }

        UE_FATAL("Cannot load unsupported shader type '%s'.", extension);
        return nullptr;
    }

    const std::vector<std::shared_ptr<Scene>>& Resources::GetAllScenes() {
        return this->scenes;
    }

    const std::vector<std::shared_ptr<Texture>>& Resources::GetNewTextures() {
        return this->newTextures;
    }

    const std::vector<std::shared_ptr<Shader>>& Resources::GetNewShaders() {
        return this->newShaders;
    }

    void Resources::Update() {
        this->newTextures.clear();
        this->newShaders.clear();

        for (size_t i = 0; i < this->scenes.size(); i++) {
            if (this->scenes[i].use_count() <= 2) {
                auto& sceneToRemove = this->scenes[i];

                for (auto& pair : this->scenePaths) {
                    if (pair.second == sceneToRemove) {
                        this->scenePaths.erase(pair.first);
                        break;
                    }
                }

                this->scenes.erase(this->scenes.begin() + i);
                i--;
            }
        }

        for (size_t i = 0; i < this->textures.size(); i++) {
            if (this->textures[i].use_count() <= 2) {
                auto& textureToRemove = this->textures[i];

                for (auto& pair : this->texturePaths) {
                    if (pair.second == textureToRemove) {
                        this->texturePaths.erase(pair.first);
                        break;
                    }
                }

                this->textures.erase(this->textures.begin() + i);
                i--;
            }
        }

        for (size_t i = 0; i < this->shaders.size(); i++) {
            if (this->shaders[i].use_count() <= 2) {
                auto& shaderToRemove = this->shaders[i];

                for (auto& pair : this->shaderPaths) {
                    if (pair.second == shaderToRemove) {
                        this->shaderPaths.erase(pair.first);
                        break;
                    }
                }

                this->shaders.erase(this->shaders.begin() + i);
                i--;
            }
        }
    }
}  // namespace UniverseEngine
