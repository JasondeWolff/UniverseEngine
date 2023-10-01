#include "MeshSimplifier.h"

namespace UniverseEngine {
    MeshSimplifier::MeshSimplifier(const std::vector<UniverseEngine::Vertex>& vertices,
                                   const std::vector<uint32_t>& indices) {
        this->vertices.reserve(vertices.size());
        for (auto& vertex : vertices) {
            MeshSimplifier::Vertex simplifiedVertex{};
            simplifiedVertex.p = vertex.position;
            this->vertices.emplace_back(simplifiedVertex);
        }
        this->triangles.reserve(indices.size() / 3);
        for (size_t i = 0; i < indices.size(); i += 3) {
            MeshSimplifier::Triangle triangle{};
            triangle.v[0] = indices[i + 0];
            triangle.v[1] = indices[i + 1];
            triangle.v[2] = indices[i + 2];
            this->triangles.emplace_back(triangle);
        }
    }

    Mesh MeshSimplifier::BuildSimplified(float percentage) {
        int targetTriangleCount =
            static_cast<int>(static_cast<float>(this->triangles.size()) * glm::clamp(percentage, 0.0f, 1.0f));

        int deletedTriangleCount = 0;
        std::vector<int> deleted0, deleted1;
        int triangleCount = static_cast<int>(this->triangles.size());

        for (size_t iteration = 0; iteration < 100; iteration++) {
            if (targetTriangleCount >= triangleCount - deletedTriangleCount)
                break;

            if (iteration % 5 == 0) {
                this->CompactMesh(static_cast<int>(iteration));
            }

            for (size_t i = 0; i < this->triangles.size(); i++) {
                this->triangles[i].dirty = false;
            }

            double threshold = 0.000000001 * pow(double(iteration + 3), 7.0);

            for (size_t i = 0; i < this->triangles.size(); i++) {
                Triangle& triangle = this->triangles[i];
                if (triangle.err[3] > threshold || triangle.deleted || triangle.dirty)
                    continue;

                for (size_t j = 0; j < 3; j++) {
                    if (triangle.err[j] < threshold) {
                        int i0 = triangle.v[j];
                        Vertex& v0 = this->vertices[i0];
                        int i1 = triangle.v[(j + 1) % 3];
                        Vertex& v1 = this->vertices[i1];

                        if (v0.border != v1.border)
                            continue;

                        glm::vec3 p;
                        this->CalculateError(i0, i1, p);

                        deleted0.resize(v0.tcount);
                        deleted1.resize(v1.tcount);

                        if (this->IsFlipped(p, i0, i1, v0, v1, deleted0) ||
                            this->IsFlipped(p, i1, i0, v1, v0, deleted1))
                            continue;

                        v0.p = p;
                        v0.q = v1.q + v0.q;
                        int tstart = static_cast<int>(this->refs.size());

                        this->UpdateTriangles(i0, v0, deleted0, deletedTriangleCount);
                        this->UpdateTriangles(i0, v1, deleted1, deletedTriangleCount);

                        int tcount = static_cast<int>(this->refs.size()) - tstart;

                        if (tcount <= v0.tcount) {
                            if (tcount)
                                memcpy(&this->refs[v0.tstart], &this->refs[tstart],
                                       tcount * sizeof(Ref));
                        } else {
                            v0.tstart = tstart;
                        }

                        v0.tcount = tcount;
                        break;
                    }
                }

                if (targetTriangleCount >= triangleCount - deletedTriangleCount)
                    break;
            }
        }

        this->CleanupMesh();

        Mesh simplifiedMesh{};
        simplifiedMesh.name = "SIMPLIFIED";
        simplifiedMesh.vertices.reserve(this->vertices.size());
        for (size_t i = 0; i < this->vertices.size(); i++) {
            UniverseEngine::Vertex vertex{};
            vertex.position = this->vertices[i].p;
            simplifiedMesh.vertices.emplace_back(vertex);
        }
        simplifiedMesh.indices.reserve(this->triangles.size() * 3);
        for (size_t i = 0; i < this->triangles.size(); i++) {
            if (!this->triangles[i].deleted) {
                simplifiedMesh.indices.push_back(this->triangles[i].v[0]);
                simplifiedMesh.indices.push_back(this->triangles[i].v[1]);
                simplifiedMesh.indices.push_back(this->triangles[i].v[2]);
            }
        }
        return Mesh(std::move(simplifiedMesh));
    }

    void MeshSimplifier::UpdateTriangles(int i0, Vertex& v, std::vector<int>& deleted,
                                         int& deleted_triangles) {
        glm::vec3 p;
        for (size_t k = 0; k < v.tcount; k++) {
            Ref& ref = this->refs[v.tstart + k];
            Triangle& triangle = this->triangles[ref.tid];
            if (triangle.deleted)
                continue;

            if (deleted[k]) {
                triangle.deleted = true;
                deleted_triangles++;
                continue;
            }

            triangle.v[ref.tvertex] = i0;
            triangle.dirty = true;
            triangle.err[0] = this->CalculateError(triangle.v[0], triangle.v[1], p);
            triangle.err[1] = this->CalculateError(triangle.v[1], triangle.v[2], p);
            triangle.err[2] = this->CalculateError(triangle.v[2], triangle.v[0], p);
            triangle.err[3] = glm::min(triangle.err[0], glm::min(triangle.err[1], triangle.err[2]));
            this->refs.push_back(ref);
        }
    }

    bool MeshSimplifier::IsFlipped(glm::vec3 p, int i0, int i1, Vertex& v0, Vertex& v1,
                                   std::vector<int>& deleted) {
        int borderCount = 0;
        for (size_t k = 0; k < v0.tcount; k++) {
            Triangle& triangle = this->triangles[this->refs[v0.tstart + k].tid];
            if (triangle.deleted)
                continue;

            int s = this->refs[v0.tstart + k].tvertex;
            int id1 = triangle.v[(s + 1) % 3];
            int id2 = triangle.v[(s + 2) % 3];

            if (id1 == i1 || id2 == i1) {
                borderCount++;
                deleted[k] = 1;
                continue;
            }

            glm::vec3 d1 = glm::normalize(this->vertices[id1].p - p);
            glm::vec3 d2 = glm::normalize(this->vertices[id2].p - p);

            if (fabs(glm::dot(d1, d2)) > 0.999)
                return true;

            glm::vec3 n = glm::normalize(glm::cross(d1, d2));
            deleted[k] = 0;
            if (glm::dot(n, triangle.n) < 0.2)
                return true;
        }
        return false;
    }

    void MeshSimplifier::CompactMesh(int iteration) {
        if (iteration > 0) {
            int dst = 0;
            for (size_t i = 0; i < this->triangles.size(); i++) {
                if (!this->triangles[i].deleted) {
                    this->triangles[dst++] = triangles[i];
                }
            }
            this->triangles.resize(dst);
        }

        for (size_t i = 0; i < this->vertices.size(); i++) {
            this->vertices[i].tstart = 0;
            this->vertices[i].tcount = 0;
        }
        for (size_t i = 0; i < this->triangles.size(); i++) {
            Triangle& triangle = this->triangles[i];
            this->vertices[triangle.v[0]].tcount++;
            this->vertices[triangle.v[1]].tcount++;
            this->vertices[triangle.v[2]].tcount++;
        }
        int tstart = 0;
        for (size_t i = 0; i < this->vertices.size(); i++) {
            Vertex& vertex = this->vertices[i];
            vertex.tstart = tstart;
            tstart += vertex.tcount;
            vertex.tcount = 0;
        }

        this->refs.resize(this->triangles.size() * 3);
        for (size_t i = 0; i < this->triangles.size(); i++) {
            Triangle& triangle = this->triangles[i];
            for (size_t j = 0; j < 3; j++) {
                Vertex& vertex = this->vertices[triangle.v[j]];
                this->refs[vertex.tstart + vertex.tcount].tid = static_cast<int>(i);
                this->refs[vertex.tstart + vertex.tcount].tvertex = static_cast<int>(j);
                vertex.tcount++;
            }
        }

        if (iteration == 0) {
            std::vector<int> vcount, vids;

            for (size_t i = 0; i < this->vertices.size(); i++) {
                this->vertices[i].border = 0;
            }

            for (size_t i = 0; i < this->vertices.size(); i++) {
                Vertex& vertex = this->vertices[i];

                vcount.clear();
                vids.clear();
                for (size_t j = 0; j < vertex.tcount; j++) {
                    int k = this->refs[vertex.tstart + j].tid;
                    Triangle& triangle = this->triangles[k];

                    for (size_t k = 0; k < 3; k++) {
                        int ofs = 0;
                        int id = triangle.v[k];
                        while (ofs < vcount.size()) {
                            if (vids[ofs] == id)
                                break;
                            ofs++;
                        }

                        if (ofs == vcount.size()) {
                            vcount.push_back(1);
                            vids.push_back(id);
                        } else {
                            vcount[ofs]++;
                        }
                    }
                }

                for (size_t j = 0; j < vcount.size(); j++) {
                    if (vcount[j] == 1) {
                        this->vertices[vids[j]].border = 1;
                    }
                }
            }

            for (size_t i = 0; i < this->vertices.size(); i++) {
                this->vertices[i].q = SymetricMatrix(0.0);
            }

            for (size_t i = 0; i < this->triangles.size(); i++) {
                Triangle& triangle = this->triangles[i];
                glm::vec3 n, p[3];
                for (size_t j = 0; j < 3; j++) {
                    p[j] = this->vertices[triangle.v[j]].p;
                }
                n = glm::cross(p[1] - p[0], p[2] - p[0]);
                n = glm::normalize(n);
                triangle.n = n;
                for (size_t j = 0; j < 3; j++) {
                    this->vertices[triangle.v[j]].q =
                        this->vertices[triangle.v[j]].q +
                        SymetricMatrix(n.x, n.y, n.z, -glm::dot(n, p[0]));
                }
            }

            for (size_t i = 0; i < this->triangles.size(); i++) {
                Triangle& triangle = this->triangles[i];
                glm::vec3 p;
                for (size_t j = 0; j < 3; j++) {
                    triangle.err[j] =
                        this->CalculateError(triangle.v[j], triangle.v[(j + 1) % 3], p);
                    triangle.err[3] =
                        glm::min(triangle.err[0], glm::min(triangle.err[1], triangle.err[2]));
                }
            }
        }
    }

    void MeshSimplifier::CleanupMesh() {
        int dst = 0;
        for (size_t i = 0; i < this->vertices.size(); i++) {
            this->vertices[i].tcount = 0;
        }
        for (size_t i = 0; i < this->triangles.size(); i++) {
            if (!triangles[i].deleted) {
                Triangle& triangle = this->triangles[i];
                this->triangles[dst++] = triangle;
                this->vertices[triangle.v[0]].tcount = 1;
                this->vertices[triangle.v[1]].tcount = 1;
                this->vertices[triangle.v[2]].tcount = 1;
            }
        }
        this->triangles.resize(dst);
        dst = 0;
        for (size_t i = 0; i < this->vertices.size(); i++) {
            if (this->vertices[i].tcount) {
                this->vertices[i].tstart = dst;
                this->vertices[dst].p = this->vertices[i].p;
                dst++;
            }
        }
        for (size_t i = 0; i < this->triangles.size(); i++) {
            Triangle& triangle = this->triangles[i];
            triangle.v[0] = this->vertices[triangle.v[0]].tstart;
            triangle.v[1] = this->vertices[triangle.v[1]].tstart;
            triangle.v[2] = this->vertices[triangle.v[2]].tstart;
        }
        this->vertices.resize(dst);
    }

    double MeshSimplifier::VertexError(const SymetricMatrix& q, double x, double y, double z) {
        return q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x + q[4] * y * y +
               2 * q[5] * y * z + 2 * q[6] * y + q[7] * z * z + 2 * q[8] * z + q[9];
    }

    double MeshSimplifier::CalculateError(int idV1, int idV2, glm::vec3& pResult) {
        SymetricMatrix q = this->vertices[idV1].q = this->vertices[idV2].q;
        bool border = this->vertices[idV1].border & this->vertices[idV2].border;
        double error = 0.0;
        double det = q.det(0, 1, 2, 1, 4, 5, 2, 5, 7);

        if (det != 0.0 && !border) {
            pResult.x = static_cast<float>(- 1.0 / det * q.det(1, 2, 3, 4, 5, 6, 5, 7, 8));
            pResult.y = static_cast<float>(1.0 / det * q.det(0, 2, 3, 1, 5, 6, 2, 7, 8));
            pResult.z = static_cast<float>(-1.0 / det * q.det(0, 1, 3, 1, 4, 6, 2, 5, 8));
            error = VertexError(q, pResult.x, pResult.y, pResult.z);
        } else {
            glm::vec3 p1 = this->vertices[idV1].p;
            glm::vec3 p2 = this->vertices[idV2].p;
            glm::vec3 p3 = (p1 + p2) * 0.5f;
            double error1 = VertexError(q, p1.x, p1.y, p1.z);
            double error2 = VertexError(q, p2.x, p2.y, p2.z);
            double error3 = VertexError(q, p3.x, p3.y, p3.z);
            error = glm::min(error1, glm::min(error2, error3));
            if (error1 = error)
                pResult = p1;
            if (error2 = error)
                pResult = p2;
            if (error3 = error)
                pResult = p3;
        }
        return error;
    }

    MeshSimplifier::SymetricMatrix::SymetricMatrix(double c) {
        for (size_t i = 0; i < 10; i++) {
            this->m[i] = c;
        }
    }

    MeshSimplifier::SymetricMatrix::SymetricMatrix(double m11, double m12, double m13, double m14,
                                                   double m22, double m23, double m24, double m33,
                                                   double m34, double m44) {
        m[0] = m11;
        m[1] = m12;
        m[2] = m13;
        m[3] = m14;
        m[4] = m22;
        m[5] = m23;
        m[6] = m24;
        m[7] = m33;
        m[8] = m34;
        m[9] = m44;
    }

    MeshSimplifier::SymetricMatrix::SymetricMatrix(double a, double b, double c, double d) {
        m[0] = a * a;
        m[1] = a * b;
        m[2] = a * c;
        m[3] = a * d;
        m[4] = b * b;
        m[5] = b * c;
        m[6] = b * d;
        m[7] = c * c;
        m[8] = c * d;
        m[9] = d * d;
    }

    double MeshSimplifier::SymetricMatrix::operator[](int c) const {
        return m[c];
    }

    double MeshSimplifier::SymetricMatrix::det(int a11, int a12, int a13, int a21, int a22, int a23,
                                               int a31, int a32, int a33) {
        double det = m[a11] * m[a22] * m[a33] + m[a13] * m[a21] * m[a32] +
                     m[a12] * m[a23] * m[a31] - m[a13] * m[a22] * m[a31] -
                     m[a11] * m[a23] * m[a32] - m[a12] * m[a21] * m[a33];
        return det;
    }

    const MeshSimplifier::SymetricMatrix MeshSimplifier::SymetricMatrix::operator+(
        const SymetricMatrix& n) const {
        return SymetricMatrix(m[0] + n[0], m[1] + n[1], m[2] + n[2], m[3] + n[3], m[4] + n[4],
                              m[5] + n[5], m[6] + n[6], m[7] + n[7], m[8] + n[8], m[9] + n[9]);
    }

    MeshSimplifier::SymetricMatrix& MeshSimplifier::SymetricMatrix::operator+=(
        const SymetricMatrix& n) {
        m[0] += n[0];
        m[1] += n[1];
        m[2] += n[2];
        m[3] += n[3];
        m[4] += n[4];
        m[5] += n[5];
        m[6] += n[6];
        m[7] += n[7];
        m[8] += n[8];
        m[9] += n[9];
        return *this;
    }
}  // namespace UniverseEngine