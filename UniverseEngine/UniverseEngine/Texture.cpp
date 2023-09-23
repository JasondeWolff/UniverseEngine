#include "Texture.h"

namespace UniverseEngine {
    Texture::Texture() : name("default"), data(nullptr), width(0), height(0) {
    }

    Texture::Texture(const std::string& name, unsigned char* data, unsigned width, unsigned height)
        : name(name), data(data), width(width), height(height) {
    }

    Texture::Texture(Texture&& other) noexcept
        : name(other.name), data(other.data), width(other.width), height(other.height), renderable(std::move(other.renderable)) {
        other.data = nullptr;
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        this->name = other.name;
        this->data = other.data;
        this->width = other.width;
        this->height = other.height;
        this->renderable = std::move(other.renderable);

        other.data = nullptr;
        return *this;
    }

    Texture::~Texture() {
        free(this->data);
    }

    void Texture::ClearCPUData() {
        free(this->data);
        this->data = nullptr;
    }
}  // namespace UniverseEngine