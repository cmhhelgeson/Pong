#version 460 core

uniform sampler2D TEX_SAMPLER; //samples a texture
out vec4 color;
in vec4 fColor;
in vec2 fTexCoords;
void main() {
  color = texture(TEX_SAMPLER, fTexCoords);
}
