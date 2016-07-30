#include "skybox_system.h"
#include "components.h"
#include "fc_tools.h"

#include <numeric>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLSL_LOCATION_POSITION 0

void SkyboxSystem::init()
{
        log_info("SkyboxSystem", "init entities: %u", m_entities.size());
        for (auto &entity : m_entities) {
                auto skybox = get_component<Skybox>(entity);

                skybox->indices.resize(skybox->faces_ids.size() * 3);
                std::iota(skybox->indices.begin(), skybox->indices.end(), 0);

                Vertices vbo_vertices{};

                for (const auto &face_id : skybox->faces_ids) {
                        auto face = get_face(face_id);
                        vbo_vertices.push_back(get_vertex(face.fv0.vertex_id));
                        vbo_vertices.push_back(get_vertex(face.fv1.vertex_id));
                        vbo_vertices.push_back(get_vertex(face.fv2.vertex_id));
                }

                glGenVertexArrays(1, &skybox->VAO);
                glGenBuffers(1, &skybox->VBO_position);
                glGenBuffers(1, &skybox->EBO);

                glBindVertexArray(skybox->VAO);

                // indices, elements
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox->EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, skybox->indices.size() * sizeof(GLuint), &skybox->indices[0], GL_STATIC_DRAW);

                // position
                glBindBuffer(GL_ARRAY_BUFFER, skybox->VBO_position);
                glBufferData(GL_ARRAY_BUFFER, vbo_vertices.size() * sizeof(Vertex), &vbo_vertices[0], GL_STREAM_DRAW);

                glVertexAttribPointer(GLSL_LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
                glEnableVertexAttribArray(0);

                glBindVertexArray(0);
        }
}

void SkyboxSystem::render(std::unique_ptr<Shader> const &shader)
{
        for (auto &entity : m_entities) {
                auto skybox = get_component<Skybox>(entity);

                glDepthMask(GL_FALSE); // remember to turn depth writing off

                GLint u_texture_location = shader->location_uniform(skybox->texture.uniform_name);
                glActiveTexture(GL_TEXTURE0);
                glUniform1i(u_texture_location, 0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture.id);

                glActiveTexture(GL_TEXTURE0);

                glBindVertexArray(skybox->VAO);
                glDrawElements(GL_TRIANGLES, skybox->indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
                glDepthMask(GL_TRUE);
        }
}
