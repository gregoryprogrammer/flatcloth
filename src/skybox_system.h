#ifndef FLATCLOTH_SKYBOX_SYSTEM_H
#define FLATCLOTH_SKYBOX_SYSTEM_H

#include "ecs.h"
#include "shader.h"

#include <memory>

class SkyboxSystem : public System
{
public:
    void init();
    void render(std::unique_ptr<Shader> const &shader);
};

#endif // FLATCLOTH_SKYBOX_SYSTEM_H
