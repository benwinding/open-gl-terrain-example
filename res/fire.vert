#version 330

layout (location = 0) in vec3 a_vertex; 
layout (location = 1) in vec3 a_normal; 
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

float rand(float seed) {
    return fract(sin(dot(vec2(seed, seed), vec2(12.9898,78.233))) * 43758.5453);
}

void main(void)
{
    float randY = rand(time);
    float randX = rand(time + 1000.f);
    vec4 randPos = vec4(0, time, 0, 0);
    gl_Position = projection * view * model * vec4(a_vertex, 1.0) + randPos;
    TexCoords = a_tex_coord;
    Normal = mat3(transpose(inverse(view))) * a_normal;
    FragPos = vec3(gl_Position);
}
