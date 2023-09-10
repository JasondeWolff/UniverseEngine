#pragma once

namespace UniverseEngine {
    struct Texture {
        Texture() : name("default"), data(nullptr), width(0), height(0) {
        }
        Texture(const std::string& name, unsigned char* data, unsigned width, unsigned height)
            : name(name), data(data), width(width), height(height) {
        }

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;
        Texture(Texture&& other) noexcept
            : name(other.name), data(other.data), width(other.width), height(other.height) {
            other.data = nullptr;
        }
        Texture& operator=(Texture&& other) noexcept {
            this->name = other.name;
            this->data = other.data;
            this->width = other.width;
            this->height = other.height;

            other.data = nullptr;
            return *this;
        }

        ~Texture() {
            delete this->data;
        }

        std::string name;
        unsigned char* data;
        unsigned width;
        unsigned height;
    };
}  // namespace UniverseEngine