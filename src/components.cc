#include "components.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 CAMERA_get_view(Entity entity)
{
        auto camera = get_component<Camera>(entity);
        auto transform = get_component<Transform>(entity);
        return glm::lookAt(transform->position, transform->position + camera->front, camera->up);
}

glm::vec3 CAMERA_get_pos(Entity entity)
{
        auto transform = get_component<Transform>(entity);
        return transform->position;
}

float CAMERA_fov(Entity entity)
{
        auto camera = get_component<Camera>(entity);
        return camera->fov;
}

void CAMERA_toggle_freefly(Entity entity)
{
        auto camera = get_component<Camera>(entity);
        camera->freefly = 1 - camera->freefly;
}

void TRANSFORM_rotate_x(Entity entity, float degrees)
{
        auto transform = get_component<Transform>(entity);
        transform->rotation.x += degrees;
}

void TRANSFORM_rotate_y(Entity entity, float degrees)
{
        auto transform = get_component<Transform>(entity);
        transform->rotation.y += degrees;
}

void TRANSFORM_rotate_z(Entity entity, float degrees)
{
        auto transform = get_component<Transform>(entity);
        transform->rotation.z += degrees;
}

void TRANSFORM_move_to(Entity entity, Vertex position)
{
        auto transform = get_component<Transform>(entity);
        transform->position = position;
}

void TRANSFORM_set_color(Entity entity, Color color)
{
        auto transform = get_component<Transform>(entity);
        transform->color = color;
}

void TRANSFORM_set_origin(Entity entity, Vertex origin)
{
        auto transform = get_component<Transform>(entity);
        transform->origin = origin;
}

void DYNAMIC_BODY_translate(Entity entity, Vertex translation)
{
        auto dynamic_body = get_component<DynamicBody>(entity);
        for (auto dynamic_id : dynamic_body->dynamics_ids) {
                dynamic_move_by(dynamic_id, translation);
        }
}

void DYNAMIC_BODY_move_to(Entity entity, Vertex vertex)
{
        auto dynamic_body = get_component<DynamicBody>(entity);
        for (auto dynamic_id : dynamic_body->dynamics_ids) {
                dynamic_move_to(dynamic_id, vertex);
        }
}

void DYNAMIC_BODY_add_force(Entity entity, Vertex force)
{
        auto dynamic_body = get_component<DynamicBody>(entity);
        for (auto &dynamic_id : dynamic_body->dynamics_ids) {
                dynamic_add_force(dynamic_id, force);
        }
}

void DYNAMIC_BODY_set_force(Entity entity, Vertex force)
{
        auto dynamic_body = get_component<DynamicBody>(entity);
        for (auto &dynamic_id : dynamic_body->dynamics_ids) {
                dynamic_set_force(dynamic_id, force);
        }
}
