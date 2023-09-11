#include "Graphics.h"

namespace UniverseEngine {
    Graphics::Graphics() {
        this->window = std::move(std::unique_ptr<Window>(new Window("Universe Engine")));
    }

    const Window& Graphics::GetWindow() const {
        return *this->window.get();
    }

    void Graphics::Update() {
        this->window->Update();
    }
}  // namespace UniverseEngine