#include <filesystem>
namespace fs = std::filesystem;

#include "Engine.h"
#include "Logging.h"
#include "MathUtil.h"
#include "Resources.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace UniverseEngine {
    Handle<Model> Resources::LoadOBJ(const fs::path& filePath) {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = (fs::path(".") / filePath.parent_path()).string();
        reader_config.triangulate = true;
        reader_config.triangulation_method = "simple";
        reader_config.vertex_color = true;

        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(filePath.string(), reader_config)) {
            UE_ASSERT_MSG(reader.Error().empty(), "Failed to load '%s'. (%s)", filePath.c_str(),
                          reader.Error().c_str());
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        Model parsedModel{};
        parsedModel.name = filePath.filename().string();

        for (auto& material : materials) {
            Material parsedMaterial;
            parsedMaterial.name = material.name;
            parsedMaterial.baseColor =
                glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0);
            if (!material.diffuse_texname.empty()) {
                parsedMaterial.baseColorMap = std::make_optional(
                    Engine::GetResources().LoadTexture(material.diffuse_texname));
            } else {
                parsedMaterial.baseColorMap = std::nullopt;
            }
            parsedModel.materials.emplace_back(parsedMaterial);
        }

        for (auto& shape : shapes) {
            auto& mesh = shape.mesh;

            // TODO: Support mesh instancing
            Mesh parsedMesh{};
            parsedMesh.materialIdx = static_cast<size_t>(std::max(mesh.material_ids[0], 0));

            size_t index_offset = 0;
            for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(mesh.num_face_vertices[f]);
                UE_ASSERT(fv == 3);

                for (size_t v = 0; v < fv; v++) {
                    Vertex parsedVertex{};
                    tinyobj::index_t idx = mesh.indices[index_offset + v];

                    parsedVertex.position.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    parsedVertex.position.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    parsedVertex.position.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                    if (idx.normal_index >= 0) {
                        parsedVertex.normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
                        parsedVertex.normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
                        parsedVertex.normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    } else {
                        parsedVertex.normal = Up();
                    }

                    if (idx.texcoord_index >= 0) {
                        parsedVertex.texCoord.x =
                            attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                        parsedVertex.texCoord.y =
                            attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    }

                    parsedVertex.color.r = attrib.colors[3 * size_t(idx.vertex_index) + 0];
                    parsedVertex.color.g = attrib.colors[3 * size_t(idx.vertex_index) + 1];
                    parsedVertex.color.b = attrib.colors[3 * size_t(idx.vertex_index) + 2];

                    // TODO: Use proper indices
                    parsedMesh.vertices.emplace_back(parsedVertex);
                    parsedMesh.indices.emplace_back(parsedMesh.vertices.size() - 1);
                }
                index_offset += fv;
            }

            parsedModel.meshes.emplace_back(parsedMesh);
        }

        Handle<Model> handle = this->models->Alloc();
        this->models->Value(handle).Value() = parsedModel;
        return handle;
    }
}  // namespace UniverseEngine