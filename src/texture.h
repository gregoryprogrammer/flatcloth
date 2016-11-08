#ifndef FLATCLOTH_TEXTURE_H
#define FLATCLOTH_TEXTURE_H

#include "opengl.h"

#include <string>

struct Texture {
    GLuint id {0};
    int w {0};
    int h {0};
};

struct TextureGL {
    GLuint id {0};
    GLenum target {GL_TEXTURE_2D};
    std::string uniform_name {""};
};

Texture get_texture(const std::string &filename);
Texture get_cube_texture(const std::string &path);

#endif // FLATCLOTH_TEXTURE_H
