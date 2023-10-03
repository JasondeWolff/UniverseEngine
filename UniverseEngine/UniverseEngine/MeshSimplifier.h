#pragma once

#include "Mesh.h"

namespace UniverseEngine {
    class MeshSimplifier {
    public:
        MeshSimplifier(const std::string& name, size_t materialIdx,
                       const std::vector<UniverseEngine::Vertex>& vertices,
                       const std::vector<uint32_t>& indices);

        Mesh BuildSimplified(float percentage);

    private:
        struct SymetricMatrix {
            SymetricMatrix(double c = 0);
            SymetricMatrix(double m11, double m12, double m13, double m14, double m22, double m23,
                           double m24, double m33, double m34, double m44);
            SymetricMatrix(double a, double b, double c, double d);

            double operator[](int c) const;

            double det(int a11, int a12, int a13, int a21, int a22, int a23, int a31, int a32,
                       int a33);

            const SymetricMatrix operator+(const SymetricMatrix& n) const;
            SymetricMatrix& operator+=(const SymetricMatrix& n);

            double m[10];
        };
        struct Triangle {
            int v[3];
            int va[3];
            double err[4];
            bool deleted, dirty;
            glm::vec3 n;
        };
        struct Vertex {
            glm::vec3 p;
            int tstart, tcount;
            SymetricMatrix q;
            int border;
        };
        struct Ref {
            int tid, tvertex;
        };

        static const bool smartLink = true;

        std::vector<Triangle> triangles;
        std::vector<Vertex> vertices;
        std::vector<Ref> refs;

        std::string name;
        size_t materialIdx;

        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec4> tangents;
        std::vector<glm::vec4> colors;

        void CompactMesh(int iteration);
        void UpdateTriangles(int i0, int ia0, Vertex& v, std::vector<int>& deleted, int& deleted_triangles);
        bool IsFlipped(glm::vec3 p, int i0, int i1, Vertex& v0, Vertex& v1, std::vector<int>& deleted);
        static double VertexError(const MeshSimplifier::SymetricMatrix& q, double x, double y, double z);
        double CalculateError(int idV1, int idV2, glm::vec3& pResult);
        glm::vec3 BarycentricCoords(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b,
                                    const glm::vec3& c);
        void CleanupMesh();
    };
}  // namespace UniverseEngine