#include "shader.h"
#include "fc_tools.h"

#include <fstream>

void shader_program_print_log(GLuint sprogram)
{
        char log[2048];
        int i;
        glGetProgramInfoLog(sprogram, 2048, &i, log);
        log[i] = '\0';

        if (strcmp(log, "") != 0) {
                log_error("ShaderLog", "%s", log);
                exit(1);
        }
}

GLuint compile_shader(const std::string &filename, const GLenum &type)
{
        log_info("ShaderCompile", "loading from file: %s", filename.c_str());

        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

        GLuint shader = glCreateShader(type);
        const char *data = content.c_str();

        log_debug("ShaderCompile", "compiling: %s", filename.c_str());
        glShaderSource(shader, 1, &data, NULL);
        glCompileShader(shader);

        char log[2048];
        int logsize;
        glGetShaderInfoLog(shader, 2048, &logsize, log);
        log[logsize] = '\0';

        if (strcmp(log, "") != 0) {
                log_error("ShaderCompile", "%s", log);
                exit(1);
        }
        return shader;
}

Shader::Shader(const std::string &vsfile, const std::string &fsfile)
{
        load(vsfile, fsfile);
        link();
}

void Shader::load(const std::string &vsfile, const std::string &fsfile)
{
        m_sprogram = glCreateProgram();

        m_vshader = compile_shader(vsfile, GL_VERTEX_SHADER);
        glAttachShader(m_sprogram, m_vshader);
        shader_program_print_log(m_sprogram);

        m_fshader = compile_shader(fsfile, GL_FRAGMENT_SHADER);
        glAttachShader(m_sprogram, m_fshader);
        shader_program_print_log(m_sprogram);
}

void Shader::link()
{
        glLinkProgram(m_sprogram);
        shader_program_print_log(m_sprogram);
}

void Shader::use()
{
        glUseProgram(m_sprogram);
        shader_program_print_log(m_sprogram);
}

void Shader::destroy()
{
        glDeleteProgram(m_sprogram);
        glDeleteShader(m_fshader);
        glDeleteShader(m_vshader);
}

void Shader::uniform_float(const std::string &name, float *val)
{
        GLint location = location_uniform(name);
        if (location == -1) {
                log_warning("Shader", "shader has no float uniform named %s", name.c_str());
        } else {
                glUniform1fv(location, 1, (float *)val);
        }
}

void Shader::uniform_vec3(const std::string &name, float *val)
{
        GLint location = location_uniform(name);
        if (location == -1) {
                log_warning("Shader", "shader has no vec3 uniform named %s", name.c_str());
        } else {
                glUniform3fv(location, 1, (float *)val);
        }
}

void Shader::uniform_vec4(const std::string &name, float *val)
{
        GLint location = location_uniform(name);
        if (location == -1) {
                log_warning("Shader", "shader has no vec4 uniform named %s", name.c_str());
        } else {
                glUniform4fv(location, 1, (float *)val);
        }
}

void Shader::uniform_mat4(const std::string &name, float *val)
{
        GLint location = location_uniform(name);
        if (location == -1) {
                log_warning("Shader", "shader has no mat4 uniform named %s", name.c_str());
        } else {
                glUniformMatrix4fv(location, 1, GL_FALSE, (float *) val);
        }
}

void Shader::bind_texture(const std::string &name, int index, GLint texture)
{
        GLint location = location_uniform(name);
        if (location == -1) {
                log_warning("Shader", "shader has no texture uniform named %s", name.c_str());
        } else {
                glActiveTexture(GL_TEXTURE0 + index);
                glBindTexture(GL_TEXTURE_2D, texture);
                glUniform1i(location, index); // 0 base, 2 normal, 4 shadow
        }
}

GLint Shader::location_attrib(const std::string &name)
{
        return glGetAttribLocation(m_sprogram, name.c_str());
}

GLint Shader::location_uniform(const std::string &name)
{
        return glGetUniformLocation(m_sprogram, name.c_str());
}
