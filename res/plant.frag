#version 330

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 diffuse;

void main(void)
{
    FragColor = vec4(diffuse, 1.0);
}
