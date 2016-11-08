#include "render_system.h"
#include "components.h"
#include "fc_tools.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLSL_LOCATION_POSITION 0
#define GLSL_LOCATION_TEXCOORD 1

void RenderSystem::init()
{
    log_info("RenderSystem", "init entities: %u", m_entities.size());
    for (auto &entity : m_entities) {

        auto mesh = get_component<Mesh>(entity);
        auto mesh_gl = get_component<MeshGL>(entity);

        std::vector<GLuint> indices {};
        indices.reserve(mesh->faces_ids.size() * 3);

        Vertices vbo_vertices;
        Texcoords vbo_texcoords;

        vbo_vertices.reserve(mesh->faces_ids.size() * 3);
        vbo_texcoords.reserve(mesh->faces_ids.size() * 3);

        for (const auto &vertex_id : mesh->vertices_ids) {
            auto vertex = get_vertex(vertex_id);
            vbo_vertices.push_back(vertex);
        }

        for (const auto &texcoord_id : mesh->texcoords_ids) {
            auto texcoord = get_texcoord(texcoord_id);
            vbo_texcoords.push_back(texcoord);
        }

        assert(vbo_vertices.size() == vbo_texcoords.size());

        for (const auto &face_id : mesh->faces_ids) {
            auto face = get_face(face_id);
            indices.push_back(face.fv0.index);
            indices.push_back(face.fv1.index);
            indices.push_back(face.fv2.index);
        }

        glGenVertexArrays(1, &mesh_gl->VAO);
        glGenBuffers(1, &mesh_gl->VBO_position);
        glGenBuffers(1, &mesh_gl->VBO_texcoord);
        glGenBuffers(1, &mesh_gl->EBO);

        glBindVertexArray(mesh_gl->VAO);

        // indices, elements
        mesh_gl->indices = indices;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_gl->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_gl->indices.size() * sizeof(GLuint), &mesh_gl->indices[0], GL_STATIC_DRAW);

        // layout:
        // x, y, z
        // tx, ty

        // position
        glBindBuffer(GL_ARRAY_BUFFER, mesh_gl->VBO_position);
        glBufferData(GL_ARRAY_BUFFER, vbo_vertices.size() * sizeof(Vertex), &vbo_vertices[0], GL_STREAM_DRAW);

        glVertexAttribPointer(GLSL_LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
        glEnableVertexAttribArray(GLSL_LOCATION_POSITION);

        // texcoord
        glBindBuffer(GL_ARRAY_BUFFER, mesh_gl->VBO_texcoord);
        glBufferData(GL_ARRAY_BUFFER, vbo_texcoords.size() * sizeof(Texcoord), &vbo_texcoords[0], GL_STATIC_DRAW);

        glVertexAttribPointer(GLSL_LOCATION_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), (GLvoid *) 0);
        glEnableVertexAttribArray(GLSL_LOCATION_TEXCOORD);

        glBindVertexArray(0);
    }
}

void RenderSystem::update_vbo()
{
    for (auto &entity : m_entities) {
        auto mesh = get_component<Mesh>(entity);
        auto mesh_gl = get_component<MeshGL>(entity);

        Vertices vbo_vertices;
        vbo_vertices.reserve(mesh->faces_ids.size() * 3);

        for (const auto &vertex_id : mesh->vertices_ids) {
            auto vertex = get_vertex(vertex_id);
            vbo_vertices.push_back(vertex);
        }

        // position
        glBindBuffer(GL_ARRAY_BUFFER, mesh_gl->VBO_position);
        glBufferData(GL_ARRAY_BUFFER, vbo_vertices.size() * sizeof(Vertex), &vbo_vertices[0], GL_STREAM_DRAW);
    }
}

void RenderSystem::draw(std::unique_ptr<Shader> const &shader)
{
    for (auto &entity : m_entities) {
        auto mesh = get_component<Mesh>(entity);
        auto mesh_gl = get_component<MeshGL>(entity);
        auto transform = get_component<Transform>(entity);

        auto u_model = glm::translate(glm::mat4(1.0f), transform->position - transform->origin);

        glm::mat4 origin_0 = glm::translate(glm::mat4(1.0f), transform->origin);
        glm::mat4 rot_x = glm::rotate(glm::mat4(1.0f), glm::radians(transform->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rot_y = glm::rotate(glm::mat4(1.0f), glm::radians(transform->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rot_z = glm::rotate(glm::mat4(1.0f), glm::radians(transform->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 origin_1 = glm::translate(glm::mat4(1.0f), -transform->origin);

        u_model = u_model * origin_0 * rot_x * rot_y * rot_z * origin_1;

        shader->uniform_mat4("u_model", glm::value_ptr(u_model));

        auto u_color = transform->color;
        shader->uniform_vec4("u_color", glm::value_ptr(u_color));

        for (size_t i = 0; i < mesh->textures.size(); ++i) {
            GLint u_texture_location = shader->location_uniform(mesh->textures[i].uniform_name);
            glActiveTexture(GL_TEXTURE0 + i);
            glUniform1i(u_texture_location, i);
            glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        if (mesh_gl->two_sided) {
            glDisable(GL_CULL_FACE);
        } else {
            glEnable(GL_CULL_FACE);
        }

        glBindVertexArray(mesh_gl->VAO);
        glDrawElements(GL_TRIANGLES, mesh_gl->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
