#include "Texture.h"

namespace UniverseEngine {
    Texture::Texture() : name("default"), data(nullptr), width(0), height(0), mips(0) {
    }

    Texture::Texture(const std::string& name, unsigned char* data, unsigned width, unsigned height,
                     TextureType type, ImageDimensions dimensions, unsigned depth, unsigned mips)
        : name(name),
          data(data),
          width(width),
          height(height),
          depth(depth),
          mips(mips),
          type(type),
          dimensions(dimensions) {
    }

    Texture::Texture(Texture&& other) noexcept
        : name(other.name),
          data(other.data),
          width(other.width),
          height(other.height),
          depth(other.depth),
          mips(other.mips),
          type(other.type),
          dimensions(other.dimensions),
          renderable(std::move(other.renderable)) {
        other.data = nullptr;
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        this->name = other.name;
        this->data = other.data;
        this->width = other.width;
        this->height = other.height;
        this->depth = other.depth;
        this->mips = other.mips;
        this->type = other.type;
        this->dimensions = other.dimensions;
        this->renderable = std::move(other.renderable);

        other.data = nullptr;
        return *this;
    }

    Texture::~Texture() {
        free(this->data);
    }

    const TextureRenderable& Texture::Renderable() const {
        return *this->renderable;
    }

    void Texture::ClearCPUData() {
        free(this->data);
        this->data = nullptr;
    }
}  // namespace UniverseEngine