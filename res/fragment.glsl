#version 330 core

in vec3 sColour;
in vec2 sTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
   FragColor = texture(uTexture, sTexCoord) * vec4(sColour, 1.0f);
}