#ifndef FLATCLOTH_ENGINE_H
#define FLATCLOTH_ENGINE_H

#include "shader.h"

#include <SDL2/SDL.h>
#include <map>
#include <memory>

class Engine {
    SDL_Window *m_window{nullptr};
    SDL_GLContext m_glcontext{0};
    std::map<std::string, std::unique_ptr<Shader>> m_shaders{};

    void init();
    void cleanup();
    int poll_events();
    int logic();
    void render();

public:
    Engine() = default;
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;
    void loop();
};

#endif // FLATCLOTH_ENGINE_H
