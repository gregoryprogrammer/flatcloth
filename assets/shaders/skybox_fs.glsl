#version 130

//in vec3 v_texcoord;
varying vec3 v_texcoord;

uniform samplerCube u_texture_skybox_0;

void main()
{
        gl_FragColor = texture(u_texture_skybox_0, v_texcoord);
}

