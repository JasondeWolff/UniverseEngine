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
        std::shared_ptr<Scene> LoadScene(const std::filesystem::path& filePath);
        std::shared_ptr<Scene> CreateScene(Mesh&& mesh);
        std::shared_ptr<Texture> LoadTexture(const std::filesystem::path& filePath);
        std::shared_ptr<Shader> LoadShader(const std::filesystem::path& filePath);

        const std::vector<std::shared_ptr<Scene>>& GetAllScenes();

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

        std::vector<std::shared_ptr<Texture>> newTextures;
        std::vector<std::shared_ptr<Shader>> newShaders;

        std::shared_ptr<Scene> LoadGLTF(const std::filesystem::path& filePath);
        std::shared_ptr<Scene> LoadOBJ(const std::filesystem::path& filePath);
        std::shared_ptr<Scene> LoadUSD(const std::filesystem::path& filePath);
        std::shared_ptr<Texture> LoadIMG(const std::filesystem::path& filePath);
        std::shared_ptr<Shader> LoadShaderSource(const std::filesystem::path& filePath);
        Mesh CreatePlane(int width = 1, int height = 1, int widthSegments = 1, int heightSegments = 1);
    };
}  // namespace UniverseEngine