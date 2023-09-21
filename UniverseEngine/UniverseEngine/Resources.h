#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>

#include "AtomicPool.h"
#include "Pool.h"
#include "Scene.h"
#include "Shader.h"

namespace UniverseEngine {
    class Engine;

    class Resources {
    public:
        Handle<Scene> LoadScene(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadTexture(const std::filesystem::path& filePath);
        Handle<Shader> LoadShader(const std::filesystem::path& filePath);

        void DeleteScene(Handle<Scene> hScene);
        void DeleteShader(Handle<Shader> hShader);

        OptionalPtr<Scene> GetScene(Handle<Scene> hScene);
        OptionalPtr<Texture> GetTexture(AtomicHandle<Texture> hTexture);
        OptionalPtr<Shader> GetShader(Handle<Shader> hShader);

        const std::vector<Handle<Shader>>& GetNewShaders();

    private:
        friend class Engine;
        Resources();

        void Update();

        std::unique_ptr<Pool<Scene>> scenes;
        std::unique_ptr<AtomicPool<Texture>> textures;
        std::unique_ptr<Pool<Shader>> shaders;

        std::unordered_map<std::filesystem::path, Handle<Scene>> scenePaths;
        std::unordered_map<std::filesystem::path, WeakAtomicHandle<Texture>> texturePaths;
        std::unordered_map<std::filesystem::path, Handle<Shader>> shaderPaths;

        std::vector<Handle<Shader>> newShaders;

        Handle<Scene> LoadOBJ(const std::filesystem::path& filePath);
        Handle<Scene> LoadGLTF(const std::filesystem::path& filePath);
        Handle<Scene> LoadUSD(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadIMG(const std::filesystem::path& filePath);
        Handle<Shader> LoadShaderSource(const std::filesystem::path& filePath);
    };
}  // namespace UniverseEngine