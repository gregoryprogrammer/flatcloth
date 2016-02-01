#version 130

// #define POSITION 0
// #define TEXCOORD 1

// layout (location = POSITION) in vec3 a_position;
// layout (location = TEXCOORD) in vec2 a_texcoord;
attribute vec3 a_position;
attribute vec2 a_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

// out vec2 v_texcoord;
varying vec2 v_texcoord;

void main()
{
        gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
        v_texcoord = a_texcoord;
}

