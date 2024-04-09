#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 sTexCoord;

uniform mat4 uProjection;
uniform mat4 uModel;
uniform mat4 uView;

void main() {
   gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);

   sTexCoord = aTexCoord;
}
