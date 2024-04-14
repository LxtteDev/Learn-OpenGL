#version 330 core

in vec2 sTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
   FragColor = texture(uTexture, sTexCoord);
   // FragColor = vec4(0.77, 0.52, 0.75, 1.0);
}