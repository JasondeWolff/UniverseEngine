#pragma once

#include "AtomicPool.h"
#include "Pool.h"

namespace UniverseEngine {
	class Engine;

	//class Material;
	//class Mesh;
	//class Model;
	//class Texture;

	class Resources {
	public:
		/*Handle<Model> LoadModel(const char* filePath);
		void FreeModel(Handle<Model> handle);

		OptionalPtr<Model> GetModel(Handle<Model> handle);
		OptionalPtr<Mesh> GetMesh(Handle<Mesh> handle);
		OptionalPtr<Material> GetMaterial(Handle<Material> handle);
		OptionalPtr<Texture> GetTexture(Handle<Texture> handle);*/

	private:
		friend class Engine;
		Resources();

		/*Pool<Model> models;
		Pool<Mesh> meshes;
		Pool<Material> materials;
		Pool<Texture> textures;*/
	};
}