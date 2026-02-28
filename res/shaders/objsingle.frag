#version 330

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material {
    sampler2D texture;
    float shininess;
    vec3 specular;
    vec3 diffuse;
    vec3 ambient;
}; 

uniform Material material;

void main(void)
{
    vec3 colour = texture(material.texture, TexCoords).rgb;
    FragColor = vec4(colour, 1.0);
}
