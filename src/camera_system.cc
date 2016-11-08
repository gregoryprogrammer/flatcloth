#include "camera_system.h"
#include "components.h"
#include "input.h"

#include <glm/gtc/matrix_transform.hpp>

void CameraSystem::init()
{
        update_camera_vectors();
}

void CameraSystem::update_camera_vectors()
{
    for (auto &entity : m_entities) {
        auto camera = get_component<Camera>(entity);

        glm::vec3 front;
        glm::vec3 front_fps;

        //// TODO fps-like
        front.x = glm::cos(glm::radians(camera->pyr.y)) * glm::cos(glm::radians(camera->pyr.x));
        front.y = glm::sin(glm::radians(camera->pyr.x));
        front.z = glm::sin(glm::radians(camera->pyr.y)) * glm::cos(glm::radians(camera->pyr.x));

        front_fps.x = glm::cos(glm::radians(camera->pyr.y));
        front_fps.z = glm::sin(glm::radians(camera->pyr.y));

        camera->front = glm::normalize(front);
        camera->front_fps = glm::normalize(front_fps);

        camera->right = glm::normalize(glm::cross(camera->front, camera->world_up));
        camera->up = glm::normalize(glm::cross(camera->right, camera->front));
    }
}

void CameraSystem::temporary_steering(float dt)
{
    for (auto &entity : m_entities) {
        auto transform = get_component<Transform>(entity);
        auto camera = get_component<Camera>(entity);
        auto velocity = 4.0f * dt;
        auto front = camera->front;

        if (key_pressed(SDL_SCANCODE_W)) {
            transform->position += front * velocity;
        } else if (key_pressed(SDL_SCANCODE_S)) {
            transform->position -= front * velocity;
        }

        if (key_pressed(SDL_SCANCODE_A)) {
            transform->position -= camera->right * velocity;
        } else if (key_pressed(SDL_SCANCODE_D)) {
            transform->position += camera->right * velocity;
        }

        if (camera->freefly == 0) {
            front = camera->front;
            transform->position.y = camera->height;
        }
    }
}

void CameraSystem::mouse_movement(int dx, int dy)
{
    for (auto &entity : m_entities) {
        auto camera = get_component<Camera>(entity);
        auto sensitivity = 0.25f;

        camera->pyr.y += dx * sensitivity;
        camera->pyr.x -= dy * sensitivity;

        camera->pyr.x = UNWIND(camera->pyr.x);
        camera->pyr.y = UNWIND(camera->pyr.y);
        camera->pyr.z = UNWIND(camera->pyr.z);

        camera->pyr.x = glm::clamp(camera->pyr.x, PITCH_MIN, PITCH_MAX);
        camera->pyr.y = glm::clamp(camera->pyr.y, YAW_MIN, YAW_MAX);
        camera->pyr.z = glm::clamp(camera->pyr.z, ROLL_MIN, ROLL_MAX);
    }
}

