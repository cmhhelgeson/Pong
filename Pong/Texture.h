#pragma once
#include "core.h"

struct Texture {
	const char* filepath;
	uint32_t id;
	int width;
	int height;
};


static GLenum textureSlots[6] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5 };

Texture* initTexturePointer(std::string s, int slot);
Texture initTexture(std::string s, int slot);
void unbindTexture(Texture* tex);
void uploadTexture(uint32_t id, const char* name, uint32_t tex_slot);