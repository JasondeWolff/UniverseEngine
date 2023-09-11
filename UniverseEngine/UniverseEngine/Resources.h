#pragma once

#include <filesystem>
#include <memory>

#include "AtomicPool.h"
#include "Model.h"
#include "Pool.h"
#include "Scene.h"

namespace UniverseEngine {
    class Engine;

    class Resources {
    public:
        Handle<Model> LoadModel(const std::filesystem::path& filePath);
        Handle<Scene> LoadScene(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadTexture(const std::filesystem::path& filePath);
        void FreeModel(Handle<Model> handle);

		OptionalPtr<Model> GetModel(Handle<Model> handle);
		OptionalPtr<Texture> GetTexture(AtomicHandle<Texture> handle);

    private:
        friend class Engine;
        Resources();

        // TODO: Atomic materials (will require hashable materials)
        std::unique_ptr<Pool<Model>> models;
        std::unique_ptr<Pool<Scene>> scenes;
        std::unique_ptr<AtomicPool<Texture>> textures;

        Handle<Model> LoadOBJ(const std::filesystem::path& filePath);
        Handle<Scene> LoadUSD(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadIMG(const std::filesystem::path& filePath);
    };
}  // namespace UniverseEngine