#include <filesystem>
namespace fs = std::filesystem;

#include "Resources.h"

#include <tinyusdz/tinyusdz.hh>

namespace UniverseEngine {
    Handle<Model> Resources::LoadUSD(const fs::path& filePath) {
        return Handle<Model>::Invalid();
    }
}  // namespace UniverseEngine