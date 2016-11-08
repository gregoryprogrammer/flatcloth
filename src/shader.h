#ifndef FLATCLOTH_SHADER_H
#define FLATCLOTH_SHADER_H

#include "opengl.h"
#include <string>
#include <SDL2/SDL.h>

class Shader {
    GLuint m_vshader = 0;
    GLuint m_fshader = 0;
    GLuint m_sprogram = 0;

    void load(const std::string &vsfile, const std::string &fsfile);
    void link();
    Shader(const Shader &);

public:
    Shader(const std::string &vsfile, const std::string &fsfile);

    void use();
    void destroy();

    void uniform_float(const std::string &name, float *val);
    void uniform_vec3(const std::string &name, float *val);
    void uniform_vec4(const std::string &name, float *val);
    void uniform_mat4(const std::string &name, float *val);

    void bind_texture(const std::string &name, int index, GLint texture);
    GLint location_attrib(const std::string &name);
    GLint location_uniform(const std::string &name);
};

#endif // FLATCLOTH_SHADER_H
