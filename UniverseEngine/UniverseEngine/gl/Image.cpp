#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Image.h"
#include "GlGraphicsFormat.h"

namespace UniverseEngine {
    Image::Image(const std::string& name, std::shared_ptr<LogicalDevice> device,
                 const PhysicalDevice& physicalDevice, uint32_t width, uint32_t height, uint32_t mips, ImageUsage usage, GraphicsFormat format, uint32_t arrayLayers)
        : width(width), height(height), mips(mips), arrayLayers(arrayLayers), format(format) {
        glGenTextures(1, &this->texture);
        this->identifier = (arrayLayers == 1) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
        glBindTexture(this->identifier, this->texture);

        glTexParameteri(this->identifier, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(this->identifier, GL_TEXTURE_WRAP_T, GL_REPEAT);
        if (this->identifier == GL_TEXTURE_CUBE_MAP)
            glTexParameteri(this->identifier, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexParameteri(this->identifier, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(this->identifier, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        if (this->identifier == GL_TEXTURE_2D) {
            glTexImage2D(GL_TEXTURE_2D, 0,
                         GlGraphicsFormat::ToInternalFormat(format), width, height, 0,
                         GlGraphicsFormat::ToFormat(format), GlGraphicsFormat::ToType(format), 0);
        } else {
            for (size_t i = 0; i < arrayLayers; i++) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i), 0,
                             GlGraphicsFormat::ToInternalFormat(format), width, height, 0,
                             GlGraphicsFormat::ToFormat(format), GlGraphicsFormat::ToType(format),
                             0);
            }
        }
        glGenerateMipmap(this->identifier);
    }

    Image::Image(void* data, uint32_t width, uint32_t height, uint32_t mips, GraphicsFormat format)
        : width(width), height(height), mips(mips), arrayLayers(1), format(format), identifier(GL_TEXTURE_2D) {
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GlGraphicsFormat::ToInternalFormat(format), width, height, 0,
                     GlGraphicsFormat::ToFormat(format), GlGraphicsFormat::ToType(format),
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    Image::~Image() {
        glDeleteTextures(1, &this->texture);
    }

    unsigned Image::GetTexture() const {
        return this->texture;
    }

    GLenum Image::Identifier() const {
        return this->identifier;
    }
}
#endif