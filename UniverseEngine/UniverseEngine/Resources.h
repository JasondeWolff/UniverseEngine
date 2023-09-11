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

		OptionalPtr<Scene> GetScene(Handle<Scene> handle);
		OptionalPtr<Texture> GetTexture(AtomicHandle<Texture> handle);

    private:
        friend class Engine;
        Resources();

        // TODO: Atomic materials (will require hashable materials)
        std::unique_ptr<Pool<Scene>> scenes;
        std::unique_ptr<AtomicPool<Texture>> textures;

        Handle<Scene> LoadOBJ(const std::filesystem::path& filePath);
        Handle<Scene> LoadUSD(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadIMG(const std::filesystem::path& filePath);
    };
}  // namespace UniverseEngine