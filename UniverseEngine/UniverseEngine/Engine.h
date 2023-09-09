#pragma once

#include <memory>

#include "Resources.h"

namespace UniverseEngine {
    class Engine {
    public:
        Engine();

        static Resources& GetResources();

    private:
        static Engine* gInstance;

        std::unique_ptr<Resources> resources;
    };
}  // namespace UniverseEngine