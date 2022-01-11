#version 460 core

uniform sampler2D uTextures[2];

out vec4 color;
in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;
void main() {
	int index = int(fTexID);
	color = texture(uTextures[index], fTexCoords);
	//color =  vec4(fTexID, fTexID, fTexID, 1.0);
	//color = mix(texture(TEX_SAMPLER, fTexCoords), texture(TEX_SAMPLER_1, fTexCoords), lerp);
  //color = texture(TEX_SAMPLER, fTexCoords);
}
