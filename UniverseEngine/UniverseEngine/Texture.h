#pragma once

#include "TextureRenderable.h"
#include "Image.h"

namespace UniverseEngine {
    class Graphics;
    class MaterialRenderable;

    enum class TextureType {
        SRGB,
        UNORM
    };

    struct Texture {
        Texture();
        Texture(const std::string& name, unsigned char* data, unsigned width, unsigned height,
                TextureType type, ImageDimensions dimensions, unsigned depth, unsigned mips = 1);

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;
        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        ~Texture();

        std::string name;
        unsigned char* data;
        unsigned width;
        unsigned height;
        unsigned depth;
        unsigned mips;

        ImageDimensions dimensions;
        TextureType type;

        const TextureRenderable& Renderable() const;

    private:
        friend class Graphics;
        friend class MaterialRenderable;
        std::unique_ptr<TextureRenderable> renderable;

        void ClearCPUData();
    };
}  // namespace UniverseEngine