#version 330 core

layout (location = 0) in vec3 aPosition;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoord;

out vec3 sFragPosition;
out vec2 sTexCoord;
out vec3 sNormal;

uniform mat4 uProjection;
uniform mat4 uModel;
uniform mat4 uView;

void main() {
   gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
   // gl_Position = vec4(aPosition, 1.0);

   // sFragPosition = vec3(uModel * vec4(aPosition, 1.0));
   // sTexCoord = aTexCoord;
   // sNormal = mat3(transpose(inverse(uModel))) * aNormal; 
}
