#include "texture.h"
#include "fc_tools.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <algorithm>

static std::map<std::string, Texture> textures;

static Texture load_texture(const std::string &filename)
{
    int w, h, comp;
    unsigned char *image;

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_rgb_alpha);

    if (image == nullptr) {
        log_error("null image: %s", filename.c_str());
        exit(1);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
             (GLvoid *) image);
    stbi_image_free(image);

    log_info("loaded: %s, id: %d", filename.c_str(), texture_id);
    glBindTexture(GL_TEXTURE_2D, 0);

    auto texture = Texture {texture_id, w, h};

    return texture;
}

static Texture load_cube_texture(std::vector<std::string> faces)
{
    GLuint texture_id;
    int w, h, comp;
    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    for (GLuint i = 0; i < faces.size(); ++i) {
        unsigned char *image = stbi_load(faces[i].c_str(), &w, &h, &comp, STBI_rgb);

        if (image == nullptr) {
            log_error("null image: %s", faces[i].c_str());
            exit(1);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    log_info("cubetex loaded id: %d", texture_id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    auto texture = Texture {texture_id, w, h};

    return texture;
}

Texture get_texture(const std::string &filename)
{
    auto tex = textures.find(filename);
    if (tex == textures.end()) {
        auto texture = load_texture(filename);
        textures[filename] = texture;
        return texture;
    }

    return (*tex).second;
}

Texture get_cube_texture(const std::string &path)
{
    std::vector<std::string> faces {
        "/right.png",
        "/left.png",
        "/top.png",
        "/bottom.png",
        "/back.png",
        "/front.png",
    };
    std::for_each(faces.begin(), faces.end(), [&path](auto & s) {
        s = path + s;
    });

    auto tex = textures.find(path);
    if (tex == textures.end()) {
        auto texture = load_cube_texture(faces);
        textures[path] = texture;
        return texture;
    }

    return (*tex).second;
}
