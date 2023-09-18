#include <fstream>
#include <sstream>
namespace fs = std::filesystem;

#include "GraphicsAPI.h"
#include "Resources.h"

namespace UniverseEngine {
    Handle<Shader> Resources::LoadShaderSource(const fs::path& filePath) {
        const fs::path shaderFile = filePath.parent_path() / GraphicsAPI::ShaderDir() / filePath.filename();

        Shader parsedShader;

        std::ifstream t(shaderFile);
        std::stringstream buffer;
        buffer << t.rdbuf();
        parsedShader.name = shaderFile.filename().string();
        parsedShader.sourceCode = buffer.str();

        std::string extension = filePath.extension().string();
        if (extension == ".vert")
            parsedShader.type = ShaderType::VERTEX;
        if (extension == ".frag")
            parsedShader.type = ShaderType::FRAGMENT;

        Handle<Shader> handle = this->shaders->Alloc();
        this->shaders->Value(handle).Value() = std::move(parsedShader);
        return handle;
    }
}  // namespace UniverseEngine