#version 460 core
layout (location=0) in vec3 vPos;
layout (location=1) in vec4 vColor;

out vec4 fColor;

void main() {
  fColor = vColor;
  gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);
}

