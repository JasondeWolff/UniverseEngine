#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Image.h"
#include "GlGraphicsFormat.h"

namespace UniverseEngine {
    Image::Image(const std::string& name, std::shared_ptr<LogicalDevice> device,
                 const PhysicalDevice& physicalDevice, uint32_t width, uint32_t height, uint32_t mips,
                 ImageUsage usage, GraphicsFormat format) : width(width), height(height), mips(mips), format(format) {
    
    }

    Image::Image(void* data, uint32_t width, uint32_t height, uint32_t mips, GraphicsFormat format)
        : width(width), height(height), mips(mips), format(format) {
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, texture);

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
}
#endif