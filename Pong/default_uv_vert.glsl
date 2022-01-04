#version 460 core
layout (location=0) in vec3 vPos;
layout (location=1) in vec4 vColor;
layout (location=2) in vec2 vTexCoords;

out vec4 fColor;
out vec2 fTexCoords;
uniform mat4 transform;
uniform mat4 proj;
uniform mat4 view;

void main() {
  fColor = vColor;
  fTexCoords = vTexCoords;
  gl_Position = proj * view * transform * vec4(vPos, 1.0);
}
