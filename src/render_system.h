#ifndef FLATCLOTH_RENDER_SYSTEM_H
#define FLATCLOTH_RENDER_SYSTEM_H

#include "ecs.h"
#include "shader.h"

#include <memory>

class RenderSystem : public System
{
public:
    void init();
    void update_vbo();
    void draw(std::unique_ptr<Shader> const &shader);
};

#endif // FLATCLOTH_RENDER_SYSTEM_H
