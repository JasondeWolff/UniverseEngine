#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>

#include "Scene.h"
#include "Shader.h"

namespace UniverseEngine {
    class Engine;

    class Resources {
    public:
        std::shared_ptr<Scene> CreateScene(Mesh&& mesh);
        std::shared_ptr<Texture> CreateTexture(
            const std::string& name, unsigned char* data, unsigned width, unsigned height,
            TextureType type, ImageDimensions dimensions = ImageDimensions::IMAGE_2D,
            unsigned depth = 1, bool allowMips = true);

        std::shared_ptr<Scene> LoadScene(const std::filesystem::path& filePath);
        std::shared_ptr<Texture> LoadTexture(const std::filesystem::path& filePath,
                                             TextureType type);
        std::shared_ptr<Shader> LoadShader(const std::filesystem::path& filePath);

        const std::vector<std::shared_ptr<Scene>>& GetAllScenes();

        const std::vector<std::shared_ptr<Scene>>& GetNewScenes();
        const std::vector<std::shared_ptr<Texture>>& GetNewTextures();
        const std::vector<std::shared_ptr<Shader>>& GetNewShaders();

    private:
        friend class Engine;
        Resources();

        void Update();

        std::vector<std::shared_ptr<Scene>> scenes;
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Shader>> shaders;

        std::unordered_map<std::filesystem::path, std::shared_ptr<Scene>> scenePaths;
        std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>> texturePaths;
        std::unordered_map<std::filesystem::path, std::shared_ptr<Shader>> shaderPaths;

        std::vector<std::shared_ptr<Scene>> newScenes;
        std::vector<std::shared_ptr<Texture>> newTextures;
        std::vector<std::shared_ptr<Shader>> newShaders;

        std::shared_ptr<Scene> LoadGLTF(const std::filesystem::path& filePath);
        std::shared_ptr<Scene> LoadOBJ(const std::filesystem::path& filePath);
        std::shared_ptr<Texture> LoadIMG(const std::filesystem::path& filePath, TextureType type);
        std::shared_ptr<Shader> LoadShaderSource(const std::filesystem::path& filePath);
        Mesh CreatePlane(int width, int height, int widthSegments, int heightSegments);
    };
}  // namespace UniverseEngine