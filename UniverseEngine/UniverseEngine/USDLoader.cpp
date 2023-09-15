#include <filesystem>
namespace fs = std::filesystem;

#include <tinyusdz/tinyusdz.hh>

#include "MathUtil.h"
#include "Resources.h"

// Include pprinter.hh and value-pprint.hh if you want to print TinyUSDZ classes/structs/enums.
// `tinyusdz::to_string()` and `std::operator<<` for TinyUSDZ classes/enums are provided separately
// for faster compilation
#include <iostream>
#include <tinyusdz/io-util.hh>
#include <tinyusdz/path-util.hh>
#include <tinyusdz/pprinter.hh>
#include <tinyusdz/tydra/render-data.hh>
#include <tinyusdz/tydra/scene-access.hh>
#include <tinyusdz/value-pprint.hh>

namespace UniverseEngine {
    Handle<Scene> Resources::LoadUSD(const fs::path& filePath) {
        //
        // Create simple material with UsdPrevieSurface.
        //
        tinyusdz::Material mat;
        mat.name = "mat";

        tinyusdz::Shader shader;  // Shader container
        shader.name = "defaultPBR";
        {
            tinyusdz::UsdPreviewSurface surfaceShader;  // Concrete Shader node object

            //
            // Asssign actual shader object to Shader::value.
            // Also do not forget set its shader node type name through Shader::info_id
            //
            shader.info_id = tinyusdz::kUsdPreviewSurface;  // "UsdPreviewSurface" token

            //
            // Currently no shader network/connection API.
            // Manually construct it.
            //
            surfaceShader.outputsSurface.set_authored(true);  // Author `token outputs:surface`

            surfaceShader.metallic = 0.3f;
            // TODO: UsdUVTexture, UsdPrimvarReader***, UsdTransform2d

            // Connect to UsdPreviewSurface's outputs:surface by setting targetPath.
            //
            // token outputs:surface = </mat/defaultPBR.outputs:surface>
            mat.surface.set(tinyusdz::Path(/* prim path */ "/mat/defaultPBR",
                                           /* prop path */ "outputs:surface"));

            //
            // Shaer::value is `value::Value` type, so can use '=' to assign Shader object.
            //
            shader.value = std::move(surfaceShader);
        }

        tinyusdz::Prim shaderPrim(shader);
        tinyusdz::Prim matPrim(mat);
        //------------------------------------------------------------------------------------------

        std::string filename = filePath.string();

        std::string err;
        std::string warn;

        std::string ext = filename.substr(filename.find_last_of(".") + 1);

        tinyusdz::Stage stage;
        stage.add_root_prim(&matPrim);


        // Auto detect USDA/USDC/USDZ
        if (ext.compare("usd") == 0) {
            bool ret = tinyusdz::LoadUSDFromFile(filename, &stage, &warn, &err);
            if (!warn.empty()) {
                std::cerr << "WARN : " << warn << "\n";
            }
            if (!err.empty()) {
                std::cerr << "Failed to load USD file: " << filename << "\n";
                std::cerr << "ERR : " << err << "\n";
                return Handle<Scene>::Invalid();
            }
        }

        std::string s = stage.ExportToString();
        std::cout << s << "\n";
        std::cout << "--------------------------------------"
                  << "\n";

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
            return Handle<Scene>::Invalid();
        }

        if (converter.GetWarning().size()) {
            std::cout << "ConvertToRenderScene warn: " << converter.GetWarning() << "\n";
        }

        std::cout << DumpRenderScene(render_scene) << "\n";

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

        Handle<Scene> handle = this->scenes->Alloc();
        this->scenes->Value(handle).Value() = std::move(USDscene);
        return handle;
    }
}  // namespace UniverseEngine