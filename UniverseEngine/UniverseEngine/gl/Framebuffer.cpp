#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../Framebuffer.h"
#include "../Image.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../RenderPass.h"

namespace UniverseEngine {
    Framebuffer::Framebuffer(std::shared_ptr<LogicalDevice> device,
                             std::vector<std::shared_ptr<Image>> images,
                             std::shared_ptr<RenderPass> renderPass)
        : device(device), images(images), renderPass(renderPass) {
        glGenFramebuffers(1, &this->framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

        unsigned colorCount = 0;
        for (size_t i = 0; i < images.size(); i++) {
            if (!IsDepthFormat(images[i]->Format())) {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (colorCount++), GL_TEXTURE_2D,
                                       images[i]->GetTexture(), 0);
            } else {
               /* glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                       GL_TEXTURE_2D, images[i]->GetTexture(), 0);*/
            }
        }

        UE_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete");
    }

    Framebuffer::Framebuffer(std::shared_ptr<LogicalDevice> device,
                             std::vector<std::shared_ptr<Image>> images,
                             std::shared_ptr<RenderPass> renderPass, unsigned fbo)
        : device(device), images(images), renderPass(renderPass), framebuffer(fbo) {
    }

    Framebuffer::Framebuffer(Framebuffer&& other) noexcept
        : device(other.device), images(other.images), renderPass(other.renderPass), framebuffer(other.framebuffer) {
        other.framebuffer = 0;
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
        this->images = other.images;
        this->renderPass = other.renderPass;
        this->framebuffer = other.framebuffer;
        other.framebuffer = 0;
        return *this;
    }

    Framebuffer::~Framebuffer() {
        if (this->framebuffer != 0) {
            glDeleteFramebuffers(1, &this->framebuffer);
        }
    }

    const std::vector<std::shared_ptr<Image>>& Framebuffer::Images() const {
        return this->images;
    }

    unsigned Framebuffer::GetFramebuffer() const {
        return this->framebuffer;
    }
}  // namespace UniverseEngine
#endif