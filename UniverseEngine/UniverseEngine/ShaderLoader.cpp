#include <fstream>
#include <sstream>
namespace fs = std::filesystem;

#include "GraphicsAPI.h"
#include "Resources.h"

namespace UniverseEngine {
    Handle<Shader> Resources::LoadShaderSource(const fs::path& filePath) {
        const fs::path shaderFile = filePath.parent_path() / GraphicsAPI::ShaderDir() / fs::path(filePath.filename().string() + GraphicsAPI::ShaderAppendix());

        Shader parsedShader;

        parsedShader.name = shaderFile.filename().string();
        std::string extension = filePath.extension().string();
        if (extension == ".vert")
            parsedShader.type = ShaderType::SHADER_VERTEX;
        if (extension == ".frag")
            parsedShader.type = ShaderType::SHADER_FRAGMENT;

        {
            std::ifstream t(shaderFile);
            std::stringstream buffer;
            buffer << t.rdbuf();
            parsedShader.sourceCode = buffer.str();
        }

        {
            std::ifstream t(shaderFile, std::ios::ate | std::ios::binary);
            size_t fileSize = (size_t)t.tellg();
            std::vector<char> data(fileSize);
            t.seekg(0);
            t.read(data.data(), fileSize);
            t.close();
            parsedShader.source = data;
        }

        Handle<Shader> handle = this->shaders->Alloc();
        this->shaders->Value(handle).Value() = std::move(parsedShader);
        return handle;
    }
}  // namespace UniverseEngine