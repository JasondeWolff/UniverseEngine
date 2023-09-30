#include "TextureRenderable.h"

namespace UniverseEngine {
    std::shared_ptr<Image> TextureRenderable::GetImage() const {
        return this->image;
    }
}