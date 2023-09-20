#include <filesystem>
namespace fs = std::filesystem;

#include "MathUtil.h"
#include "Resources.h"

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
//#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"

namespace gltf = tinygltf;

namespace UniverseEngine {
    Handle<Scene> Resources::LoadGLTF(const fs::path& filePath) {
        std::string filename = filePath.string();

        gltf::Model model;
        gltf::TinyGLTF loader;
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

        Handle<Scene> handle = this->scenes->Alloc();
        //this->scenes->Value(handle).Value() = std::move(gtlfScene);
        return handle;
    }
    
}  // namespace UniverseEngine