#version 330

layout (location = 0) in vec3 a_vertex; 
layout (location = 1) in vec3 a_normal; 
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

void main(void)
{
    gl_Position = projection * view * model * vec4(a_vertex, 1.0);
    TexCoords = a_tex_coord;
    Normal = mat3(transpose(inverse(view))) * a_normal;
    FragPos = vec3(gl_Position);
}
