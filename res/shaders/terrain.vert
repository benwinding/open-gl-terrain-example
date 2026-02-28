#version 330

layout (location = 0) in vec3 a_vertex; 
layout (location = 1) in vec3 a_normal; 
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 colorTop;
uniform vec3 colorMid;
uniform vec3 colorMin;
uniform float mixFactor1;
uniform float mixFactor2;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 TerrainColour;

#define M_PI 3.1415926535897932384626433832795

vec3 getTerrainColour() 
{
	float height = a_vertex.y;
	float minY = -0.5;
	float maxY = 0.5;
	float heightRatio = (a_vertex.y - minY) / (maxY - minY);
	heightRatio = max(min(heightRatio, 1.0), 0.0);
	vec3 outColour = vec3(0.0);
	float h1 = sin(heightRatio*1.5*M_PI + M_PI/2.0);
	float h2 = sin(heightRatio*1.5*M_PI + M_PI);
	outColour = mix(colorMid, colorMin, mixFactor1 * h1);
	outColour = mix(outColour, colorTop, mixFactor2 * h2);
	return outColour;
}

void main(void)
{
    gl_Position = projection * view * model * vec4(a_vertex, 1.0);
    TexCoords = a_tex_coord;
    Normal = mat3(transpose(inverse(view))) * a_normal;
    FragPos = vec3(gl_Position);
    TerrainColour = getTerrainColour();
}
