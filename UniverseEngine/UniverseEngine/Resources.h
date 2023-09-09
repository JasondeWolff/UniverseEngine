#pragma once

#include <filesystem>
#include <memory>

#include "AtomicPool.h"
#include "Model.h"
#include "Pool.h"

namespace UniverseEngine {
    class Engine;

    class Resources {
    public:
        Handle<Model> LoadModel(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadTexture(const std::filesystem::path& filePath);
        void FreeModel(Handle<Model> handle);

<<<<<<< HEAD
		void loadUSDModel();

		OptionalPtr<Model> GetModel(Handle<Model> handle);
		OptionalPtr<Mesh> GetMesh(Handle<Mesh> handle);
		OptionalPtr<Material> GetMaterial(Handle<Material> handle);
		OptionalPtr<Texture> GetTexture(AtomicHandle<Texture> handle);
=======
        OptionalPtr<Model> GetModel(Handle<Model> handle);
        OptionalPtr<Texture> GetTexture(AtomicHandle<Texture> handle);
>>>>>>> 62e9eae5af26075071d4b555ac28f927aa2c57f3

    private:
        friend class Engine;
        Resources();

        // TODO: Atomic materials (will require hashable materials)
        std::unique_ptr<Pool<Model>> models;
        std::unique_ptr<AtomicPool<Texture>> textures;

        Handle<Model> LoadOBJ(const std::filesystem::path& filePath);
        Handle<Model> LoadUSD(const std::filesystem::path& filePath);
        AtomicHandle<Texture> LoadIMG(const std::filesystem::path& filePath);
    };
}  // namespace UniverseEngine