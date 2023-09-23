#pragma once

#include "TextureRenderable.h"

namespace UniverseEngine {
    class Graphics;

    struct Texture {
        Texture();
        Texture(const std::string& name, unsigned char* data, unsigned width, unsigned height);

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;
        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        ~Texture();

        std::string name;
        unsigned char* data;
        unsigned width;
        unsigned height;

    private:
        friend class Graphics;
        std::unique_ptr<TextureRenderable> renderable;

        void ClearCPUData();
    };
}  // namespace UniverseEngine