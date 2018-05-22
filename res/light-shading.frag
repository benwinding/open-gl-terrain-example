#version 330

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D texture;
    float shininess;
    vec3 specular;
    vec3 diffuse;
    vec3 ambient;
}; 

struct Scene {
    int lightingMode;
    vec3 viewDir;
    vec3 viewPos;
    vec3 rotatingPosition;
};

uniform Material material;
uniform Scene scene;

vec3 ApplyDirectionalLight(Material material, Light light, vec3 lightDirection)
{
    vec3 colour = texture(material.texture, TexCoords).rgb;
          
    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse
    vec3 lightDir = normalize(-lightDirection);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = colour * material.diffuse * light.diffuse * diff;
   
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(scene.viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = colour * material.specular * light.specular * spec;

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 ApplyBlingPhong(Material material, Light light, vec3 lightPos) 
{
    vec3 colour = texture(material.texture, TexCoords).rgb;
    // ambient
    vec3 ambient = light.ambient * material.ambient;
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = colour * material.diffuse * light.diffuse * diff;
    // specular
    vec3 viewDir = normalize(scene.viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = colour * material.specular * light.specular * spec;
    // attenuation
    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance 
        + light.quadratic * (distance * distance));    
    
    diffuse *= attenuation;
    ambient *= attenuation;
    specular *= attenuation;
   
    vec3 result = ambient + diffuse + specular;
    return result;
}

void main(void)
{
    vec3 colour;
    int mode = scene.lightingMode;
    // int mode = 0;
    Light light;
    light.ambient = vec3(1,1,1);
    light.constant = 1;
    light.linear = 0.09;
    light.quadratic = 0.032;
    /*
        An overhead directional light, that emits 
        red diffuse light say RGB = (1.0, 0.5, 0.5)) and 
        blue specular light say RGB = (0.5, 0.5, 1.0)). 
        This light is fixed in world coordinates;
    */
    if (mode == 0)
    {
        light.diffuse = vec3(1.0, 0.5, 0.5);
        light.specular = vec3(0.5, 0.5, 1.0);
        vec3 direction = vec3(0, -1, 0);
        colour = ApplyDirectionalLight(material, light, direction);
    }
    /*
        A point light that is fixed to the camera (i.e. a head light), and emits green light
        (say RGB = (0.5, 1.0, 0.5));
     */
    else if (mode == 1) {
        light.diffuse = vec3(0.5,1.0,0.5);
        light.specular = vec3(0.5,1.0,0.5);        
        vec3 position = vec3(0.0,0.0,3.0);
        colour = ApplyBlingPhong(material, light, position);
    }    
    /* 
        A yellow point light that rotates around the object at a fixed speed;
     */
    else if (mode == 2) {
        light.diffuse = vec3(1,0.5,0.5);
        light.specular = vec3(1,0.5,1);
        vec3 position = scene.rotatingPosition;
        colour = ApplyBlingPhong(material, light, position);
    }
    else {
        colour = texture(material.texture, TexCoords).rgb;
    }
    FragColor = vec4(colour, 1);
}
