#include "Resources.h"

#include "Logging.h"
namespace fs = std::filesystem;

namespace UniverseEngine {
    Resources::Resources() : scenes{}, textures{}, shaders{}, scenePaths{}, texturePaths{} {
    }

    std::shared_ptr<Scene> Resources::CreateScene(Mesh&& mesh) {
        std::shared_ptr<Scene> hScene = std::make_shared<Scene>();

        hScene->meshes.emplace_back(std::move(mesh));
        hScene->materials.emplace_back(Material{});
        auto instanceRoot = hScene->hierarchy.begin();
        instanceRoot = hScene->hierarchy.insert(instanceRoot, SceneNode{});
        SceneNode node{};
        node.meshIdx = std::make_optional(0);
        hScene->hierarchy.append_child(instanceRoot, node);

        this->scenes.push_back(hScene);
        this->newScenes.push_back(hScene);
        return hScene;
    }

    std::shared_ptr<Texture> Resources::CreateTexture(const std::string& name, unsigned char* data,
                                                      unsigned width, unsigned height,
                                                      TextureType type, ImageDimensions dimensions,
                                                      unsigned depth, bool allowMips) {
        unsigned mips = static_cast<unsigned>(std::floor(std::log2(std::max(width, height)))) + 1;
        if (!allowMips)
            mips = 1;
        Texture parsedTexture(name, data, static_cast<unsigned>(width),
                              static_cast<unsigned>(height), type, dimensions, depth, mips);

        std::shared_ptr<Texture> texture = std::make_shared<Texture>(std::move(parsedTexture));
        this->newTextures.push_back(texture);
        this->textures.push_back(texture);
        return texture;
    }

    std::shared_ptr<Scene> Resources::LoadScene(const std::filesystem::path& filePath) {
        auto scene = scenePaths.find(filePath);
        if (scene != scenePaths.end())
            return scene->second;

        std::shared_ptr<Scene> hScene;

        std::string extension = filePath.extension().string();
        if (extension == ".obj")
            hScene = LoadOBJ(filePath);
        else if (extension == ".gltf")
            hScene = LoadGLTF(filePath);
        else
            UE_FATAL("Cannot load unsupported scene type '%s'.", extension);

        hScene->BuildLODS();

        this->scenes.push_back(hScene);
        this->newScenes.push_back(hScene);
        this->scenePaths.insert(std::make_pair(filePath, hScene));
        return hScene;
    }

    std::shared_ptr<Texture> Resources::LoadTexture(const fs::path& filePath, TextureType type) {
        auto texture = texturePaths.find(filePath);
        if (texture != texturePaths.end())
            return texture->second;

        static const std::string supportedExtensions[8] = {".jpg", ".png", ".tga", ".bmp",
                                                           ".psd", ".gif", ".hdr", ".pic"};

        fs::path extension = filePath.extension();
        for (size_t i = 0; i < 8; i++) {
            if (supportedExtensions[i] == extension) {
                std::shared_ptr<Texture> hTexture = LoadIMG(filePath, type);
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

        static const std::string supportedExtensions[3] = {".vert", ".frag", ".comp"};

        fs::path extension = filePath.extension();
        for (size_t i = 0; i < 3; i++) {
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

    const std::vector<std::shared_ptr<Scene>>& Resources::GetNewScenes() {
        return this->newScenes;
    }

    const std::vector<std::shared_ptr<Texture>>& Resources::GetNewTextures() {
        return this->newTextures;
    }

    const std::vector<std::shared_ptr<Shader>>& Resources::GetNewShaders() {
        return this->newShaders;
    }

    void Resources::Update() {
        this->newScenes.clear();
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
