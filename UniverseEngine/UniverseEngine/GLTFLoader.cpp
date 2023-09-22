#include <filesystem>
namespace fs = std::filesystem;

#include "MathUtil.h"
#include "Resources.h"

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
//#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#pragma warning(disable : 4018)
#pragma warning(disable : 4267)
#include "tiny_gltf.h"

namespace UniverseEngine {
    Handle<Scene> Resources::LoadGLTF(const fs::path& filePath) {
        std::string filename = filePath.string();

        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        // bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary
        // glTF(.glb)

        if (!warn.empty()) {
            printf("Warn: %s\n", warn.c_str());
        }

        if (!err.empty()) {
            printf("Err: %s\n", err.c_str());
        }

        if (!ret) {
            printf("Failed to parse glTF\n");
        }

        Scene gltfScene;
        gltfScene.name = model.scenes[0].name;

        for (auto node : model.nodes) {
            
            std::vector<Mesh> meshes;
            for (tinygltf::Mesh mesh : model.meshes) {
                Mesh UEMesh;
                UEMesh.name = mesh.name;

                //TODO Material loading

                for (auto& primitive : mesh.primitives) {
                    std::vector<glm::vec3> positions;
                    std::vector<glm::vec3> normals;
                    std::vector<glm::vec2> texCoords;
                    std::vector<glm::vec3> colors;
                    std::vector<uint32_t> indices;

                    for (auto& attribute : primitive.attributes) {
                        const auto& accessor = model.accessors[attribute.second];
                        const auto& view = model.bufferViews[accessor.bufferView];
                        const auto& buffer = model.buffers[view.buffer];

                        if (attribute.first == "POSITION") {
                            positions.reserve(accessor.count);
                            positions.resize(accessor.count);
                            memcpy(positions.data(),
                                   &buffer.data.at(view.byteOffset + accessor.byteOffset),
                                   accessor.count * sizeof(float) * 3);
                        } 
                        else if (attribute.first == "NORMAL") {
                            normals.reserve(accessor.count);
                            normals.resize(accessor.count);
                            memcpy(normals.data(),
                                   &buffer.data.at(view.byteOffset + accessor.byteOffset),
                                   accessor.count * sizeof(float) * 3);
                        } 
                        else if (attribute.first == "TEXCOORD_0") {
                            texCoords.reserve(accessor.count);
                            texCoords.resize(accessor.count);
                            memcpy(texCoords.data(),
                                   &buffer.data.at(view.byteOffset + accessor.byteOffset),
                                   accessor.count * sizeof(float) * 2);
                        } 
                        else if (attribute.first == "COLOR_0") {
                            colors.reserve(accessor.count);
                            colors.resize(accessor.count);
                            memcpy(colors.data(),
                                   &buffer.data.at(view.byteOffset + accessor.byteOffset),
                                   accessor.count * sizeof(float) * 3);

                            for (auto& color : colors) {
                                color.r /= std::numeric_limits<uint16_t>::max();
                                color.g /= std::numeric_limits<uint16_t>::max();
                                color.b /= std::numeric_limits<uint16_t>::max();
                            }
                        }
                    }

                    if (positions.size() <= 0) {
                        UE_ERROR("Failed to load position data. Mesh: %s", mesh.name.c_str());
                        return Handle<Scene>::Invalid();
                    }
                    if (normals.size() <= 0) {
                        UE_ERROR("Failed to load normal data. Mesh: %s", mesh.name.c_str());
                        return Handle<Scene>::Invalid();
                    }
                    if (texCoords.size() <= 0)
                        UE_WARNING("Failed to load texCoord data. Mesh: %s", mesh.name.c_str());
                    if (colors.size() <= 0)
                        UE_WARNING("Failed to load color data. Mesh: %s", mesh.name.c_str());

                    const auto& accessor = model.accessors[primitive.indices];
                    const auto& view = model.bufferViews[accessor.bufferView];
                    const auto& buffer = model.buffers[view.buffer];

                    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                        std::vector<uint16_t> smallIndices;
                        smallIndices.reserve(accessor.count);
                        smallIndices.resize(accessor.count);
                        memcpy(smallIndices.data(),
                               &buffer.data.at(view.byteOffset + accessor.byteOffset),
                               accessor.count * sizeof(uint16_t));

                        indices.reserve(smallIndices.size());
                        indices.resize(smallIndices.size());
                        for (size_t i = 0; i < smallIndices.size(); i++) {
                            indices[i] = static_cast<uint32_t>(smallIndices[i]);
                        }
                    } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                        memcpy(indices.data(),
                               &buffer.data.at(view.byteOffset + accessor.byteOffset),
                               accessor.count * sizeof(uint32_t));
                    } else {
                        UE_FATAL("Failed to load gltf: %s. (Model is not using ushort or uint "
                                 "for indices)",
                                 filename.c_str());
                    }

                    std::vector<Vertex> vertices;

                    for (int i = 0; i < positions.size(); i++) {
                        Vertex vert;

                        if (positions.size() > 0) {
                            vert.position = positions[i];
                        }
                        else {
                            vert.position = glm::vec3(0,0,0);
                        }
                        if (normals.size() > 0) {
                            vert.normal = normals[i];
                        } 
                        else {
                            vert.normal = glm::vec3(0, 0, 0);
                        }
                        if (texCoords.size() > 0) {
                            vert.texCoord = texCoords[i];
                        } 
                        else {
                            vert.texCoord = glm::vec2(0, 0);
                        }
                        if (colors.size() > 0) {
                            vert.color = colors[i];
                        } 
                        else {
                            vert.color = glm::vec3(0, 0, 0);
                        }

                        vertices.push_back(vert);
                    }

                    for (int i = 0; i < vertices.size(); i++) {
                        UEMesh.vertices.push_back(vertices[i]);
                    }
                    for (int i = 0; i < indices.size(); i++) {
                        UEMesh.indices.push_back(indices[i]);
                    }

                    meshes.push_back(std::move(UEMesh));
                }
            }

            gltfScene.meshes = std::move(meshes);
        }

        Handle<Scene> handle = this->scenes->Alloc();
        this->scenes->Value(handle).Value() = std::move(gltfScene);
        return handle;
    }
    
}  // namespace UniverseEngine