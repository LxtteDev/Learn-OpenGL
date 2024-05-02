#version 330 core

struct Material {
    float shininess;
    vec3 specular;
    vec3 ambient;
    vec3 diffuse;
};

struct Light {
    vec3 position;

    vec3 specular;
    vec3 ambient;
    vec3 diffuse;
};

in vec3 sFragPosition;
in vec3 sNormal;

out vec4 FragColor;

uniform vec3 uViewPosition;

uniform Light light;
uniform Material material;

void main() {
    vec3 ambient = light.ambient * material.ambient;

    vec3 normal = normalize(sNormal);
    vec3 lightingDirection = normalize(light.position - sFragPosition);
    
    float diff = max(dot(normal, lightingDirection), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 viewDirection = normalize(uViewPosition - sFragPosition);
    vec3 reflectDirection = reflect(-lightingDirection, sNormal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}