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
        reader_config.mtl_search_path =
            (fs::path(".") / filePath.parent_path()).string();
        reader_config.triangulate = true;
        reader_config.triangulation_method = "simple";
        reader_config.vertex_color = true;

        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(filePath.string(), reader_config)) {
            UE_ASSERT_MSG(reader.Error().empty(), "Failed to load '%s'. (%s)",
                          filePath.c_str(), reader.Error().c_str());
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        for (auto& shape : shapes) {
            auto& mesh = shape.mesh;

            size_t index_offset = 0;
            for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(mesh.num_face_vertices[f]);
                UE_ASSERT(fv == 3);

                for (size_t v = 0; v < fv; v++) {
                    Vertex vertex;
                    tinyobj::index_t idx = mesh.indices[index_offset + v];

                    vertex.position.x =
                        attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    vertex.position.y =
                        attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    vertex.position.z =
                        attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                    if (idx.normal_index >= 0) {
                        tinyobj::real_t nx =
                            attrib.normals[3 * size_t(idx.normal_index) + 0];
                        tinyobj::real_t ny =
                            attrib.normals[3 * size_t(idx.normal_index) + 1];
                        tinyobj::real_t nz =
                            attrib.normals[3 * size_t(idx.normal_index) + 2];
                    } else {
                        vertex.normal = Up();
                    }

                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx =
                            attrib
                                .texcoords[2 * size_t(idx.texcoord_index) + 0];
                        tinyobj::real_t ty =
                            attrib
                                .texcoords[2 * size_t(idx.texcoord_index) + 1];
                    }

                    tinyobj::real_t red =
                        attrib.colors[3 * size_t(idx.vertex_index) + 0];
                    tinyobj::real_t green =
                        attrib.colors[3 * size_t(idx.vertex_index) + 1];
                    tinyobj::real_t blue =
                        attrib.colors[3 * size_t(idx.vertex_index) + 2];
                }
                index_offset += fv;

                // per-face material
                mesh.material_ids[f];
            }
        }
    }
}  // namespace UniverseEngine