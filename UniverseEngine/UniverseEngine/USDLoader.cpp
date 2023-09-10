#include <filesystem>
namespace fs = std::filesystem;

#include <tinyusdz/tinyusdz.hh>

#include "Resources.h"

// Include pprinter.hh and value-pprint.hh if you want to print TinyUSDZ classes/structs/enums.
// `tinyusdz::to_string()` and `std::operator<<` for TinyUSDZ classes/enums are provided separately
// for faster compilation
#include <iostream>
#include <tinyusdz/pprinter.hh>
#include <tinyusdz/value-pprint.hh>
#include <tinyusdz/path-util.hh>
#include <tinyusdz/tydra/scene-access.hh>
#include <tinyusdz/tydra/render-data.hh>
#include <tinyusdz/io-util.hh>

namespace UniverseEngine {
    Handle<Model> Resources::LoadUSD(const fs::path& filePath) {
        std::string filename = filePath.u8string();

        tinyusdz::Stage stage;  // Stage in USD terminology is nearly meant for Scene in generic 3D
                                            // graphics terminology.
        std::string warn;
        std::string err;

        // Auto detect USDA/USDC/USDZ
        bool ret = tinyusdz::LoadUSDFromFile(filename, &stage, &warn, &err);
        if (warn.size()) {
            std::cout << "WARN : " << warn << "\n";
        }
        if (!ret) {
            if (!err.empty()) {
                std::cerr << "ERR : " << warn << "\n";
            }
            return Handle<Model>::Invalid();
        }

        // You can also use ExportToString() as done in pxrUSD
        std::string s = stage.ExportToString();
        std::cout << s << "\n";
        std::cout << "--------------------------------------" << "\n";

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
            std::cerr << "Failed to convert USD Stage to RenderScene: \n" << converter.GetError() << "\n";
        }

        if (converter.GetWarning().size()) {
            std::cout << "ConvertToRenderScene warn: " << converter.GetWarning() << "\n";
        }

        std::cout << DumpRenderScene(render_scene) << "\n";


        Handle<Model> handle = this->models->Alloc();
        // this->models->Value(handle).Value() = parsedModel;
        return handle;
    }
}  // namespace UniverseEngine