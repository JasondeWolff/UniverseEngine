#include <filesystem>
namespace fs = std::filesystem;

#include "MathUtil.h"
#include "Resources.h"

// Include pprinter.hh and value-pprint.hh if you want to print TinyUSDZ classes/structs/enums.
// `tinyusdz::to_string()` and `std::operator<<` for TinyUSDZ classes/enums are provided separately
// for faster compilation
#include <iostream>
#include <tinyusdz/tinyusdz.hh>
#include <tinyusdz/io-util.hh>
#include <tinyusdz/path-util.hh>
#include <tinyusdz/pprinter.hh>
#include <tinyusdz/tydra/render-data.hh>
#include <tinyusdz/tydra/scene-access.hh>
#include <tinyusdz/value-pprint.hh>

namespace UniverseEngine {
    std::shared_ptr<Scene> Resources::LoadUSD(const fs::path& filePath) {
        std::string filename = filePath.string();

        std::string err;
        std::string warn;

        std::string ext = filename.substr(filename.find_last_of(".") + 1);

        tinyusdz::Stage stage;


        // Auto detect USDA/USDC/USDZ
        if (ext.compare("usd") == 0) {
            bool ret = tinyusdz::LoadUSDFromFile(filename, &stage, &warn, &err);
            if (!warn.empty()) {
                std::cerr << "WARN : " << warn << "\n";
            }
            if (!err.empty()) {
                std::cerr << "Failed to load USD file: " << filename << "\n";
                std::cerr << "ERR : " << err << "\n";
                return nullptr;
            }
        }

        //std::string s = stage.ExportToString();
        //std::cout << s << "\n";
        //std::cout << "--------------------------------------"
        //          << "\n";

        // RenderScene: Scene graph object which is suited for GL/Vulkan renderer
        tinyusdz::tydra::RenderScene render_scene;
        tinyusdz::tydra::RenderSceneConverter converter;

        // Add base directory of .usd file to search path.
        std::string usd_basedir = tinyusdz::io::GetBaseDir(filename);
        std::cout << "Add seach path: " << usd_basedir << "\n";

        converter.set_search_paths({usd_basedir});
        // TODO: Set user-defined AssetResolutionResolver
        // AssetResolutionResolver arr;
        // converter.set_asset_resoluition_resolver(arr);

        bool ret = converter.ConvertToRenderScene(stage, &render_scene);
        if (!ret) {
            std::cerr << "Failed to convert USD Stage to RenderScene: \n"
                      << converter.GetError() << "\n";
            return nullptr;
        }

        if (converter.GetWarning().size()) {
            std::cout << "ConvertToRenderScene warn: " << converter.GetWarning() << "\n";
        }

        //std::cout << DumpRenderScene(render_scene) << "\n";

        Scene USDscene;
        USDscene.name = "aa";

        std::vector<Material> materials;
        auto materialCount = render_scene.materials.size();
        for (auto material : render_scene.materials) {
            // Load materials
            Material custommaterial;
            custommaterial.name = material.name;
            custommaterial.baseColor = glm::vec4(material.surfaceShader.diffuseColor.value[0],
                                                 material.surfaceShader.diffuseColor.value[1],
                                                 material.surfaceShader.diffuseColor.value[2],
                                                 material.surfaceShader.opacity.value);

            //  NO TEXTURE SUPPORT YET
            // custommaterial.baseColorMap = AtomicHandle<Texture>::Invalid();

            materials.push_back(custommaterial);
        }

        /*At this point render_scene hold all the information loaded from the USD
        std::cout << DumpRenderScene(render_scene) << "\n";*/
        std::vector<Mesh> meshes;

        auto meshCount = render_scene.meshes.size();
        for (auto model : render_scene.meshes) {
            Mesh parsedMesh{};
            // Load model name
            if (model.abs_name == "")
                parsedMesh.name = "usdModel";

            parsedMesh.indices = model.faceVertexIndices;

            std::vector<Vertex> vertices;
            for (int i = 0; i < model.points.size(); i++) {

                glm::vec3 pos = ToGlm(model.points[i]);

                glm::vec2 texCoord = glm::vec2(0, 0); //model.facevaryingTexcoords;

                glm::vec3 normal = ToGlm(model.facevaryingNormals[i*3]);

                Vertex v = {pos, normal, texCoord, materials[model.materialIds[i]].baseColor};
                parsedMesh.vertices.push_back(v);
            }
            USDscene.meshes.emplace_back(std::move(parsedMesh));
        }

        return std::make_shared<Scene>(std::move(USDscene));
    }
}  // namespace UniverseEngine