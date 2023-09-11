#pragma once
#include <memory>

#include "Pool.h"
#include "Model.h"

namespace UniverseEngine
{
    struct Scene 
    {
        std::string sceneName;
        std::vector<Handle<Model>> model_refs;
    };
}  // namespace UniverseEngine