#version 330

layout (location = 0) in vec3 a_vertex; 
layout (location = 1) in vec3 a_normal; 
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;
uniform int id;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

float rand(float seed) {
    return fract(sin(dot(vec2(0, seed), vec2(12.9898,78.233))) * 43758.5453);
}

float spread = 0.4;

void main(void)
{
    float curve = cos(time*2 - 1);

    float randomX = curve * spread * rand(id);
    float randomZ = curve * spread * rand(id + 1);

    vec4 randPos = vec4(randomX, time, 0, randomZ);
    mat4 model2 = model;
    gl_Position = projection * view * model2 * vec4(a_vertex, 1.0) + randPos;
    TexCoords = a_tex_coord;
    Normal = mat3(transpose(inverse(view))) * a_normal;
    FragPos = vec3(gl_Position);
}
