#version 330 core

in vec3 FragPos;

out vec4 FragColor;

uniform vec3 colour;

void main()
{
    FragColor = vec4(colour, 1);
}