#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec3 sColour;
out vec2 sTexCoord;

uniform mat4 uTransform;

void main() {
   gl_Position = uTransform * vec4(aPosition.x, aPosition.y, 0.0, 1.0);

   sColour = aColour;
   sTexCoord = aTexCoord;
}
