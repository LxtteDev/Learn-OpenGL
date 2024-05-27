#version 330 core

in vec2 sTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;
// uniform vec3 colour;

void main() {
   if (gl_FrontFacing)
      FragColor = vec4(1.0, 1.0, 1.0, 1.0);
   else
      discard;
}