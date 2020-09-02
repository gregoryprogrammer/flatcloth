#version 130

/*#define POSITION 0*/
/*layout (location = POSITION) in vec3 a_position;*/

in vec3 a_position;

//out vec3 v_texcoord;
out vec3 v_texcoord;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
        gl_Position = u_projection * u_view * vec4(a_position, 1.0f);
        v_texcoord = a_position;
}

