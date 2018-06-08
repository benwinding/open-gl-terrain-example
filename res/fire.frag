#version 330 core

in vec3 FragPos;

out vec4 FragColor;

uniform float time;
uniform int id;

float rand(float seed) {
    return fract(sin(dot(vec2(0, seed), vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    float max = 0.3;
    float min = -1;
    float intensity = (max - FragPos.y) / (max - min);
    vec4 red = vec4(1, 0, 0, 1);
    vec4 yellow = vec4(1, 1, 0, 1);
    float randomNess = rand(id);
    FragColor = mix(red, yellow, randomNess * intensity);
}