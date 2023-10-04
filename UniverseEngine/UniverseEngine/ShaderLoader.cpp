#include <fstream>
#include <sstream>
namespace fs = std::filesystem;

#include "GraphicsAPI.h"
#include "Logging.h"
#include "Resources.h"

namespace UniverseEngine {
    std::shared_ptr<Shader> Resources::LoadShaderSource(const fs::path& filePath) {
        const fs::path shaderFile =
            filePath.parent_path() / GraphicsAPI::ShaderDir() /
            fs::path(filePath.filename().string() + GraphicsAPI::ShaderAppendix());

        Shader parsedShader;

        parsedShader.name = shaderFile.filename().string();
        std::string extension = filePath.extension().string();
        if (extension == ".vert")
            parsedShader.type = ShaderType::SHADER_VERTEX;
        if (extension == ".frag")
            parsedShader.type = ShaderType::SHADER_FRAGMENT;
        if (extension == ".comp")
            parsedShader.type = ShaderType::SHADER_COMPUTE;

        {
            std::ifstream t(shaderFile);
            UE_ASSERT_MSG(t.good(), "Failed to load shader '%s'.", filePath.string().c_str());

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

        return std::make_shared<Shader>(std::move(parsedShader));
    }

    Mesh Resources::CreatePlane(int width, int height, int widthSegments, int heightSegments) {
        return Mesh();
    }
}  // namespace UniverseEngine