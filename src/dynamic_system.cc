#include "dynamic_system.h"
#include "components.h"

void DynamicSystem::pre_update()
{
    for (auto &entity : m_entities) {
        auto dynamic_body = get_component<DynamicBody>(entity);

        const auto GRAVITY = glm::vec3(0.0f, -9.81f * 5.0f, 0.0f);
        for (auto &dynamic_id : dynamic_body->dynamics_ids) {
            dynamic_add_force(dynamic_id, GRAVITY);
        }
    }
}

void DynamicSystem::post_update()
{
    for (auto &entity : m_entities) {
        const auto dynamic_body = get_component<DynamicBody>(entity);

        for (auto &dynamic_id : dynamic_body->dynamics_ids) {
            dynamic_set_force(dynamic_id, {0.0f, 0.0f, 0.0f});
        }
    }
}
