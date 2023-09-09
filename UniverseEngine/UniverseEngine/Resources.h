#pragma once

#include <memory>

#include "AtomicPool.h"
#include "Pool.h"

#include "Model.h"

namespace UniverseEngine {
	class Engine;

	class Resources {
	public:
		Handle<Model> LoadModel(const char* filePath);
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

		static std::string FileExtension(const char* filePath);

		static Handle<Model> LoadOBJ(const char* filePath);
		static Handle<Model> LoadUSD(const char* filePath);
	};
}