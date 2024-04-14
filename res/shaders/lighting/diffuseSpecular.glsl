#version 330 core

in vec3 sFragPosition;
in vec3 sNormal;

out vec4 FragColor;
  
uniform vec3 uLightingPosition;
uniform vec3 uLightingColour;
uniform vec3 uObjectColour;

uniform vec3 uViewPosition;

float specularStength = 0.5;

void main() {
    vec3 lightingDirection = normalize(uLightingPosition - sFragPosition);
    vec3 viewDirection = normalize(uViewPosition - sFragPosition);
    vec3 reflectDirection = reflect(-lightingDirection, sNormal);
    vec3 normal = normalize(sNormal);

    float diff = max(dot(normal, lightingDirection), 0.0);
    vec3 diffuse = diff * uLightingColour;

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStength * spec * uLightingColour;

    vec3 result = (diffuse + specular) * uObjectColour;
    FragColor = vec4(result, 1.0);
}