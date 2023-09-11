#pragma once

#include <filesystem>
#include <memory>

#include "AtomicPool.h"
#include "Pool.h"
#include "Scene.h"

namespace UniverseEngine {
    class Engine;

    class Resources {
    public:
        Handle<Scene> LoadScene(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadTexture(const std::filesystem::path& filePath);

        void DeleteScene(Handle<Scene> hScene);

		OptionalPtr<Scene> GetScene(Handle<Scene> hScene);
		OptionalPtr<Texture> GetTexture(AtomicHandle<Texture> hTexture);

    private:
        friend class Engine;
        Resources();

        std::unique_ptr<Pool<Scene>> scenes;
        std::unique_ptr<AtomicPool<Texture>> textures;

        Handle<Scene> LoadOBJ(const std::filesystem::path& filePath);
        Handle<Scene> LoadUSD(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadIMG(const std::filesystem::path& filePath);
    };
}  // namespace UniverseEngine