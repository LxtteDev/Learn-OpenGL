#version 330 core

in vec3 sFragPosition;
in vec3 sNormal;

out vec4 FragColor;
  
uniform vec3 uLightingPosition;
uniform vec3 uLightingColour;
uniform vec3 uObjectColour;

void main() {
    if (gl_FrontFacing) {
        vec3 lightingDirection = normalize(uLightingPosition - sFragPosition);
        vec3 normal = normalize(sNormal);

        float diff = max(dot(normal, lightingDirection), 0.0);
        vec3 diffuse = diff * uLightingColour;

        vec3 result = diffuse * uObjectColour;
        FragColor = vec4(result, 1.0);
    } else
        discard;
}