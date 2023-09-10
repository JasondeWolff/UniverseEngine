#include <filesystem>
namespace fs = std::filesystem;

#include "Resources.h"

#include <tinyusdz/tinyusdz.hh>

// Include pprinter.hh and value-pprint.hh if you want to print TinyUSDZ classes/structs/enums.
// `tinyusdz::to_string()` and `std::operator<<` for TinyUSDZ classes/enums are provided separately for faster compilation
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
        std::cout << stage.ExportToString() << "\n";


        auto rootPrims = stage.root_prims();
        // stage.metas() To get Scene metadatum,
        for (const tinyusdz::Prim& root_prim : rootPrims) {
            std::cout << root_prim.absolute_path() << "\n";

            /*if (root_prim.absolute_path().prim_part() == "/root") {
                for (auto objects : root_prim.children()) {
                    std::cout << "\n OBJECT:" << objects.element_name() << "\n";
                    auto data = objects.get_data();

                    int i = 02;
                }
            }*/

            // You can traverse Prim(scene graph object) using Prim::children()
            // See examples/api_tutorial and examples/tydra_api for details.
        }

        Handle<Model> handle = this->models->Alloc();
        //this->models->Value(handle).Value() = parsedModel;
        return handle;
    }
}  // namespace UniverseEngine