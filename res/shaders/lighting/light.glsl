#version 330 core
out vec4 FragColor;
  
uniform vec3 uLightingColour;

void main() {
    FragColor = vec4(uLightingColour, 1.0);
}