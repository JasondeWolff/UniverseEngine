#pragma once

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

		Pool<Model> models;
		Pool<Mesh> meshes;
		Pool<Material> materials;
		AtomicPool<Texture> textures;
	};
}