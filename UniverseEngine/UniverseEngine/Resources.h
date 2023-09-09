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
        void FreeModel(Handle<Model> handle);

        OptionalPtr<Model> GetModel(Handle<Model> handle);
        OptionalPtr<Mesh> GetMesh(Handle<Mesh> handle);
        OptionalPtr<Material> GetMaterial(Handle<Material> handle);
        OptionalPtr<Texture> GetTexture(AtomicHandle<Texture> handle);

    private:
        friend class Engine;
        Resources();

        std::unique_ptr<Pool<Model>> models;
        std::unique_ptr<Pool<Mesh>> meshes;
        std::unique_ptr<Pool<Material>> materials;
        std::unique_ptr<AtomicPool<Texture>> textures;

        static Handle<Model> LoadOBJ(const std::filesystem::path& filePath);
        static Handle<Model> LoadUSD(const std::filesystem::path& filePath);
    };
}  // namespace UniverseEngine