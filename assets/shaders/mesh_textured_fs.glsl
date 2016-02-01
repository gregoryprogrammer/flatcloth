#version 130

// in vec2 v_texcoord;
varying vec2 v_texcoord;

uniform sampler2D u_texture_diffuse_0;
uniform vec4 u_color;

void main()
{
        vec4 diffuse = texture(u_texture_diffuse_0, v_texcoord);
        diffuse = mix(diffuse, u_color, u_color.a);

        if (diffuse.a == 0) {
                discard;
        }

        gl_FragColor = diffuse;
}
