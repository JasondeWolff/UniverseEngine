#include "Mesh.h"

#include "MeshSimplifier.h"

namespace UniverseEngine {
    bool Mesh::HasNormals() const {
        return glm::length(glm::vec3(this->vertices[0].normal)) > 0.0f;
    }

    bool Mesh::HasTangents() const {
        return glm::length(glm::vec3(this->vertices[0].tangent)) > 0.0f;
    }

    void Mesh::GenerateNormals(bool flatShading) {
        if (flatShading) {
            for (size_t i = 0; i < this->indices.size() / 3; i++) {
                Vertex& v0 = this->vertices[this->indices[i * 3 + 0]];
                Vertex& v1 = this->vertices[this->indices[i * 3 + 1]];
                Vertex& v2 = this->vertices[this->indices[i * 3 + 2]];

                glm::vec3 n = glm::normalize(
                    glm::cross(v1.position - v0.position, v2.position - v0.position));
                v0.normal = n;
                v1.normal = n;
                v2.normal = n;
            }
        } else {
            for (auto& vertex : this->vertices)
                vertex.normal = glm::vec3(0.0f);

            for (size_t i = 0; i < this->indices.size() / 3; i++) {
                Vertex& v0 = this->vertices[this->indices[i * 3 + 0]];
                Vertex& v1 = this->vertices[this->indices[i * 3 + 1]];
                Vertex& v2 = this->vertices[this->indices[i * 3 + 2]];

                glm::vec3 n = glm::normalize(
                    glm::cross(v1.position - v0.position, v2.position - v0.position));
                v0.normal += n;
                v1.normal += n;
                v2.normal += n;
            }

            for (auto& vertex : this->vertices)
                vertex.normal = glm::normalize(vertex.normal);
        }
    }

    void Mesh::GenerateTangents() {
        std::vector<glm::vec3> tan1(this->vertices.size());
        std::vector<glm::vec3> tan2(this->vertices.size());

        for (size_t i = 0; i < this->indices.size(); i += 3) {
            uint32_t i1 = this->indices[i + 0];
            uint32_t i2 = this->indices[i + 1];
            uint32_t i3 = this->indices[i + 2];
            glm::vec3& v1 = this->vertices[i1].position;
            glm::vec3& v2 = this->vertices[i2].position;
            glm::vec3& v3 = this->vertices[i3].position;
            glm::vec2& w1 = this->vertices[i1].texCoord;
            glm::vec2& w2 = this->vertices[i2].texCoord;
            glm::vec2& w3 = this->vertices[i3].texCoord;

            float x1 = v2.x - v1.x;
            float x2 = v3.x - v1.x;
            float y1 = v2.y - v1.y;
            float y2 = v3.y - v1.y;
            float z1 = v2.z - v1.z;
            float z2 = v3.z - v1.z;

            float s1 = w2.x - w1.x;
            float s2 = w3.x - w1.x;
            float t1 = w2.y - w1.y;
            float t2 = w3.y - w1.y;

            float rdiv = s1 * t2 - s2 * t1;
            float r = (rdiv == 0.0f) ? 0.0f : 1.0f / rdiv;

            glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                           (t2 * z1 - t1 * z2) * r);
            glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                           (s1 * z2 - s2 * z1) * r);

            tan1[i1] += sdir;
            tan1[i2] += sdir;
            tan1[i3] += sdir;
            tan2[i1] += tdir;
            tan2[i2] += tdir;
            tan2[i3] += tdir;
        }

        for (size_t i = 0; i < this->vertices.size(); i++) {
            glm::vec3& n = this->vertices[i].normal;
            glm::vec3& t = tan1[i];

            glm::vec3 xyz = t - (n * glm::dot(n, t));
            if (glm::length(xyz) != 0.0f) {
                xyz = glm::normalize(xyz);
            }

            float w = (glm::dot(glm::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

            this->vertices[i].tangent = glm::vec4(xyz, w);
        }
    }

    void Mesh::ClearCPUData() {
        this->vertices.clear();
        this->vertices.shrink_to_fit();
        this->indices.clear();
        this->indices.shrink_to_fit();
    }

    Mesh Mesh::BuildFlatShaded() const {
        Mesh mesh{};
        mesh.name = this->name + "_FLAT";
        mesh.vertices = this->vertices;
        mesh.indices = this->indices;
        mesh.materialIdx = this->materialIdx;

        std::vector<bool> occupied(this->vertices.size());
        for (size_t i = 0; i < this->indices.size() / 3; i++) {
            uint32_t vi0 = this->indices[i * 3 + 0];
            uint32_t vi1 = this->indices[i * 3 + 1];
            uint32_t vi2 = this->indices[i * 3 + 2];

            if (occupied[vi0] || occupied[vi1] || occupied[vi2]) {
                mesh.indices.erase(mesh.indices.begin() + (i * 3 + 2));
                mesh.indices.erase(mesh.indices.begin() + (i * 3 + 1));
                mesh.indices.erase(mesh.indices.begin() + (i * 3 + 0));

                mesh.indices.push_back(mesh.vertices.size());
                mesh.vertices.push_back(mesh.vertices[vi0]);
                mesh.indices.push_back(mesh.vertices.size());
                mesh.vertices.push_back(mesh.vertices[vi1]);
                mesh.indices.push_back(mesh.vertices.size());
                mesh.vertices.push_back(mesh.vertices[vi2]);
            }

            occupied[vi0] = true;
            occupied[vi1] = true;
            occupied[vi2] = true;
        }

        mesh.GenerateNormals(true);
        return Mesh(std::move(mesh));
    }

    Mesh Mesh::BuildSimplified(float percentage) const {
        MeshSimplifier simplifier(this->name, this->materialIdx, this->vertices, this->indices);
        return simplifier.BuildSimplified(percentage);
    }

    LODMesh::LODMesh(Mesh&& mesh) : lods{} {
        this->lods.emplace_back(std::move(mesh));
        this->configs.emplace_back(Config{1.0f, 4.0f});
    }

    std::optional<size_t> LODMesh::BestLOD(const glm::vec3& observer, const glm::vec3& mesh) const {
        float distance = glm::distance(observer, mesh);
        for (size_t i = 0; i < this->configs.size(); i++) {
            const Config& config = this->configs[i];

            if (distance <= config.maxDistance) {
                return std::make_optional(i);
            }
        }
        return std::nullopt;
    }
}  // namespace UniverseEngine