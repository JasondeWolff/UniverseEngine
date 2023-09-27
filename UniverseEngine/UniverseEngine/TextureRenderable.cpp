#include "TextureRenderable.h"

namespace UniverseEngine {
    std::shared_ptr<Image> TextureRenderable::GetImage() {
        return this->image;
    }
}