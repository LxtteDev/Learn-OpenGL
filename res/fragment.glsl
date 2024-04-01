#version 330 core

in vec3 sColour;

out vec4 FragColor;

void main() {
   FragColor = vec4(sColour, 1.0f);
}