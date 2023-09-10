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

        // Print Stage(Scene graph)
        std::cout << tinyusdz::to_string(stage) << "\n";

        // You can also use ExportToString() as done in pxrUSD
        // std::cout << stage.ExportToString() << "\n";

        // stage.metas() To get Scene metadatum,
        for (const tinyusdz::Prim& root_prim : stage.root_prims()) {
            std::cout << root_prim.absolute_path() << "\n";
            // You can traverse Prim(scene graph object) using Prim::children()
            // See examples/api_tutorial and examples/tydra_api for details.
        }

        Handle<Model> handle = this->models->Alloc();
        // this->models->Value(handle).Value() = parsedModel;
        return handle;
    }
}  // namespace UniverseEngine