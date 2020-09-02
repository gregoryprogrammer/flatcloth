#version 130

//in vec3 v_texcoord;
in vec3 v_texcoord;

uniform samplerCube u_texture_skybox_0;

out vec4 output;

void main()
{
        output = texture(u_texture_skybox_0, v_texcoord);
}

