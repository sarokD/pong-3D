#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;  
uniform Material material;
uniform vec3 viewPos;

void main() {
    float distance = length(light.pos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.22 * distance + 0.20 * (distance * distance));

    vec3 ambient = light.color * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * attenuation * diff * material.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = attenuation * spec * material.specular;

    FragColor = vec4(ambient+diffuse+specular, 1.0);
}