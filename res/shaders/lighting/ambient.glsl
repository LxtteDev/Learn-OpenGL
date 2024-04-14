#version 330 core

out vec4 FragColor;
  
uniform vec3 uLightingColour;
uniform vec3 uObjectColour;

void main() {
    FragColor = vec4(uLightingColour * uObjectColour, 1.0);
}