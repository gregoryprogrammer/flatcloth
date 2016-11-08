#ifndef FLATCLOTH_COMPONENTS_H
#define FLATCLOTH_COMPONENTS_H

#include "fc_types.h"
#include "fc_data.h"
#include "ecs.h"
#include "opengl.h"
#include "texture.h"

#include <glm/glm.hpp>

struct Transform : public Component<Transform, 1024> {
    glm::vec3 origin {0.0f, 0.0f, 0.0f};
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation {0.0f, 0.0f, 0.0f};
    Color color {0.0f, 0.0f, 0.0f, 0.0f};
};

struct Mesh : public Component<Mesh, 1024> {
    VerticesIds vertices_ids {};
    TexcoordsIds texcoords_ids {};
    FacesIds faces_ids {};
    std::vector<TextureGL> textures {};
};

struct MeshGL : public Component<MeshGL, 1024> {
    GLuint VAO {0};
    GLuint VBO_position {0};
    GLuint VBO_texcoord {0};
    GLuint EBO {0};
    uint32_t two_sided {0};
    std::vector<GLuint> indices {};
};

struct Skybox : public Component<Skybox, 4> {
    GLuint VAO {0};
    GLuint VBO_position {0};
    GLuint EBO {0};
    TextureGL texture {};
    std::vector<FaceId> faces_ids {};
    std::vector<GLuint> indices {};
};

struct Sprite2D : public Component<Sprite2D, 1024> {
};

struct DynamicBody : public Component<DynamicBody, 1024> {
    DynamicsIds dynamics_ids {};
};

struct Camera : public Component<Camera, 8> {
    glm::vec3 front {0.0f, 0.0f, -1.0f};
    glm::vec3 front_fps {0.0f, 0.0f, -1.0f};
    glm::vec3 up {0.0f, 1.0f, 0.0f};
    glm::vec3 world_up {0.0f, 1.0f, 0.0f};
    glm::vec3 right {0.0f, 0.0f, 0.0f};
    Vertex pyr {0.0f, 0.0f, 0.0f};
    float fov {45.0f};
    float height {0.5f};
    uint32_t freefly {0};
};

struct Ball : public Component<Ball, 32> {
    float radius {0.1f};
    VertexId position_id {0};
};

glm::mat4 CAMERA_get_view(Entity entity);
glm::vec3 CAMERA_get_pos(Entity entity);
void CAMERA_toggle_freefly(Entity entity);
float CAMERA_fov(Entity entity);

void TRANSFORM_rotate_x(Entity entity, float degrees);
void TRANSFORM_rotate_y(Entity entity, float degrees);
void TRANSFORM_rotate_z(Entity entity, float degrees);

void TRANSFORM_move_to(Entity entity, Vertex position);

void TRANSFORM_set_color(Entity entity, Color color);
void TRANSFORM_set_origin(Entity entity, Vertex origin);

void DYNAMIC_BODY_translate(Entity entity, Vertex translation);
void DYNAMIC_BODY_move_to(Entity entity, Vertex vertex);
void DYNAMIC_BODY_add_force(Entity entity, Vertex force);
void DYNAMIC_BODY_set_force(Entity entity, Vertex force);

#endif // FLATCLOTH_COMPONENTS_H
