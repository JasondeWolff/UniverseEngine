#include <filesystem>
namespace fs = std::filesystem;
#include <set>

#include "Resources.h"

#pragma warning(disable : 4018)
#pragma warning(disable : 4267)
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

#include <glm/gtc/type_ptr.hpp>

#include "Engine.h"
#include "Logging.h"
#include "MathUtil.h"

namespace UniverseEngine {
    void ParseNode(const tinygltf::Model& model, size_t nodeIdx, std::set<size_t>& processedNodes,
                   tree<MeshInstance>& meshHierarchy,
                   tree<MeshInstance>::iterator_base meshHierarchyParent) {
        if (processedNodes.find(nodeIdx) != processedNodes.end())
            return;
        auto& node = model.nodes[nodeIdx];

        MeshInstance meshInstance;

        if (!node.matrix.empty()) {
            std::array<float, 16> elems;
            for (size_t i = 0; i < 16; i++) {
                elems[i] = static_cast<float>(node.matrix[i]);
            }
            meshInstance.transform = Transform(glm::make_mat4(elems.data()));
        } else {
            glm::vec3 translation{};
            if (node.translation.size() == 3)
                translation = glm::vec3(static_cast<float>(node.translation[0]),
                                  static_cast<float>(node.translation[1]),
                                  static_cast<float>(node.translation[2]));

            glm::quat rotation{};
            if (node.rotation.size() == 4)
                rotation = glm::quat(static_cast<float>(node.rotation[0]), static_cast<float>(node.rotation[1]),
                static_cast<float>(node.rotation[2]), static_cast<float>(node.rotation[3]));
           
            glm::vec3 scale{};
            if (node.scale.size() == 3)
                scale = glm::vec3(static_cast<float>(node.scale[0]), static_cast<float>(node.scale[1]),
                            static_cast<float>(node.scale[2]));

            meshInstance.transform = Transform(translation, rotation, scale);
        }

        if (node.mesh != -1) {
            meshInstance.meshIdx = static_cast<size_t>(node.mesh);
        } else {
            meshInstance.meshIdx = std::nullopt;
        }

        meshHierarchyParent = meshHierarchy.append_child(meshHierarchyParent, meshInstance);

        for (auto child : node.children) {
            ParseNode(model, static_cast<size_t>(child), processedNodes, meshHierarchy,
                      meshHierarchyParent);
        }
    }

    std::shared_ptr<Scene> Resources::LoadGLTF(const fs::path& filePath) {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err, warn;

        UE_ASSERT_MSG(loader.LoadASCIIFromFile(&model, &err, &warn, filePath.string()),
                      "Failed to load '%s'. (%s)", filePath.string().c_str(), err.c_str());

        UE_ASSERT_MSG(model.scenes.size() == 1, "GlTF cannot contain more than 1 scene.");

        Scene parsedScene{};
        parsedScene.name = model.scenes[0].name;

        for (auto& material : model.materials) {
            Material parsedMaterial{};
            parsedMaterial.name = material.name;

            UE_ASSERT_MSG(material.extensions.find("KHR_materials_pbrSpecularGlossiness") ==
                              material.extensions.end(),
                          "Specular glossiness is not supported.");

            int textureIdx = material.pbrMetallicRoughness.baseColorTexture.index;
            parsedMaterial.baseColor = glm::vec4(material.pbrMetallicRoughness.baseColorFactor[0],
                                                 material.pbrMetallicRoughness.baseColorFactor[1],
                                                 material.pbrMetallicRoughness.baseColorFactor[2],
                                                 material.pbrMetallicRoughness.baseColorFactor[3]);
            if (textureIdx != -1) {
                size_t source = model.textures[static_cast<size_t>(textureIdx)].source;
                parsedMaterial.baseColorMap = Engine::GetResources().LoadTexture(
                    filePath.parent_path() / model.images[source].uri);
            }

            textureIdx = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
            parsedMaterial.metallicFactor =
                static_cast<float>(material.pbrMetallicRoughness.metallicFactor);
            parsedMaterial.roughnessFactor =
                static_cast<float>(material.pbrMetallicRoughness.roughnessFactor);
            if (textureIdx != -1) {
                size_t source = model.textures[static_cast<size_t>(textureIdx)].source;
                parsedMaterial.metallicRoughnessMap = Engine::GetResources().LoadTexture(
                    filePath.parent_path() / model.images[source].uri);
            }

            textureIdx = material.normalTexture.index;
            parsedMaterial.normalFactor = static_cast<float>(material.normalTexture.scale);
            if (textureIdx != -1) {
                size_t source = model.textures[static_cast<size_t>(textureIdx)].source;
                parsedMaterial.normalMap = Engine::GetResources().LoadTexture(
                    filePath.parent_path() / model.images[source].uri);
            }

            textureIdx = material.emissiveTexture.index;
            parsedMaterial.emissiveFactor = glm::vec3(
                material.emissiveFactor[0], material.emissiveFactor[1], material.emissiveFactor[2]);
            if (textureIdx != -1) {
                size_t source = model.textures[static_cast<size_t>(textureIdx)].source;
                parsedMaterial.emissiveMap = Engine::GetResources().LoadTexture(
                    filePath.parent_path() / model.images[source].uri);
            }

            textureIdx = material.occlusionTexture.index;
            parsedMaterial.occlusionFactor = static_cast<float>(material.occlusionTexture.strength);
            if (textureIdx != -1) {
                size_t source = model.textures[static_cast<size_t>(textureIdx)].source;
                parsedMaterial.occlusionMap = Engine::GetResources().LoadTexture(
                    filePath.parent_path() / model.images[source].uri);
            }

            parsedScene.materials.emplace_back(std::move(parsedMaterial));
        }

        for (auto& mesh : model.meshes) {
            Mesh parsedMesh{};
            parsedMesh.name = mesh.name;

            std::vector<glm::vec3> vertexPositions;
            std::vector<glm::vec3> vertexNormals;
            std::vector<glm::vec2> vertexTexCoords;
            std::vector<glm::vec4> vertexTangents;
            std::vector<glm::vec4> vertexColors;

            auto& primitive = mesh.primitives[0];
            UE_ASSERT_MSG(primitive.mode == TINYGLTF_MODE_TRIANGLES,
                          "Only triangles are supported.");

            parsedMesh.materialIdx = static_cast<size_t>(mesh.primitives[0].material);

            for (const auto& attribute : primitive.attributes) {
                const auto& accessor = model.accessors[attribute.second];
                const auto& view = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[view.buffer];

                if (attribute.first == "POSITION") {
                    vertexPositions.reserve(accessor.count);
                    vertexPositions.resize(accessor.count);
                    memcpy(vertexPositions.data(),
                           &buffer.data.at(view.byteOffset + accessor.byteOffset),
                           accessor.count * sizeof(float) * 3);
                } else if (attribute.first == "NORMAL") {
                    vertexNormals.reserve(accessor.count);
                    vertexNormals.resize(accessor.count);
                    memcpy(vertexNormals.data(),
                           &buffer.data.at(view.byteOffset + accessor.byteOffset),
                           accessor.count * sizeof(float) * 3);
                } else if (attribute.first == "TEXCOORD_0") {
                    vertexTexCoords.reserve(accessor.count);
                    vertexTexCoords.resize(accessor.count);
                    memcpy(vertexTexCoords.data(),
                           &buffer.data.at(view.byteOffset + accessor.byteOffset),
                           accessor.count * sizeof(float) * 2);
                } else if (attribute.first == "TANGENT") {
                    vertexTangents.reserve(accessor.count);
                    vertexTangents.resize(accessor.count);
                    memcpy(vertexTangents.data(),
                           &buffer.data.at(view.byteOffset + accessor.byteOffset),
                           accessor.count * sizeof(float) * 4);
                } else if (attribute.first == "COLOR") {
                    vertexColors.reserve(accessor.count);
                    vertexColors.resize(accessor.count);
                    memcpy(vertexColors.data(),
                           &buffer.data.at(view.byteOffset + accessor.byteOffset),
                           accessor.count * sizeof(uint16_t) * 4);

                    for (auto& color : vertexColors) {
                        color.r /= std::numeric_limits<uint16_t>::max();
                        color.g /= std::numeric_limits<uint16_t>::max();
                        color.b /= std::numeric_limits<uint16_t>::max();
                        color.a /= std::numeric_limits<uint16_t>::max();
                    }
                }
            }

            parsedMesh.vertices.reserve(vertexPositions.size());
            for (size_t i = 0; i < vertexPositions.size(); i++) {
                Vertex vertex{};
                vertex.position = vertexPositions[i];

                if (vertexNormals.size() > i)
                    vertex.normal = vertexNormals[i];
                if (vertexTexCoords.size() > i)
                    vertex.texCoord = vertexTexCoords[i];
                if (vertexTangents.size() > i)
                    vertex.tangent = vertexTangents[i];
                if (vertexColors.size() > i)
                    vertex.color = vertexColors[i];

                parsedMesh.vertices.emplace_back(vertex);
            }

            const auto& accessor = model.accessors[primitive.indices];
            const auto& view = model.bufferViews[accessor.bufferView];
            const auto& buffer = model.buffers[view.buffer];

            parsedMesh.indices.reserve(accessor.count);
            parsedMesh.indices.resize(accessor.count);
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                std::vector<uint16_t> smallIndices;
                smallIndices.reserve(accessor.count);
                smallIndices.resize(accessor.count);
                memcpy(smallIndices.data(), &buffer.data.at(view.byteOffset + accessor.byteOffset),
                       accessor.count * sizeof(uint16_t));

                for (size_t i = 0; i < smallIndices.size(); i++) {
                    parsedMesh.indices[i] = static_cast<uint32_t>(smallIndices[i]);
                }
            } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                memcpy(parsedMesh.indices.data(),
                       &buffer.data.at(view.byteOffset + accessor.byteOffset),
                       accessor.count * sizeof(uint32_t));
            }

            parsedScene.meshes.emplace_back(std::move(parsedMesh));
        }

        auto meshHierarchyRoot = parsedScene.meshHierarchy.begin();
        meshHierarchyRoot = parsedScene.meshHierarchy.insert(meshHierarchyRoot, MeshInstance{});

        std::set<size_t> processedNodes{};
        for (auto& node : model.scenes[0].nodes) {
            ParseNode(model, static_cast<size_t>(node), processedNodes, parsedScene.meshHierarchy,
                      meshHierarchyRoot);
        }

        return std::make_shared<Scene>(std::move(parsedScene));
    }

}  // namespace UniverseEngine