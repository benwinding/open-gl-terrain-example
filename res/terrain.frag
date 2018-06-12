#version 330

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 TerrainColour;

out vec4 FragColor;

uniform vec3 diffuse;
uniform int debugMode;

void main(void)
{
    FragColor = vec4(TerrainColour, 1);
}
