#ifndef FLATCLOTH_FACTORY_H
#define FLATCLOTH_FACTORY_H

#include "fc_types.h"
#include "ecs.h"

Entity FACTORY_create_cloth(World *world, glm::vec3 position, int w, int h);
Entity FACTORY_create_box(World *world, Vertex position, float edge);
Entity FACTORY_create_camera(World *world, Vertex position);
Entity FACTORY_create_skybox(World *world, const std::string &path);
Entity FACTORY_create_sprite2D(World *world, const std::string &pic);
Entity FACTORY_create_floor(World *world, int w, int h, float edge);

#endif // FLATCLOTH_FACTORY_H
