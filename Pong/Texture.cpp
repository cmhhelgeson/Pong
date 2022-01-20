#include "Texture.h"

Texture* initTexturePointer(std::string s, int slot)
{
	Texture* tex = new Texture();
	tex->filepath = s.c_str();
	//Generate Texture on GPU
	glGenTextures(1, &tex->id); //maybe set multiple textures in a larger buffer in future
	glActiveTexture(textureSlots[slot]); //Different drivers have different numbers of texture slots, so past a certain number it might be helpful to have a system where texture slots are dynamically filled and emptied depending on need
	glBindTexture(GL_TEXTURE_2D, tex->id);
	//Set Texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT); //wrap texture around horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //wrap texture vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int w;
	int h;
	int channels;
	stbi_uc* image = stbi_load(tex->filepath, &w, &h, &channels, 0);
	if (image != nullptr) {
		tex->width = w;
		tex->height = h;
		//Upload image to GPU. GL_RGBA appears twice for internal and external format
		//Check for png, then upload accordingly
		if (s.substr(s.length() - size_t(3), 3) == "png") {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
				0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h,
				0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}
	}
	else {
		printf("Error could not load image '%s'", tex->filepath);
	}
	stbi_image_free(image);
	return tex;
}

Texture initTexture(std::string s, int slot)
{
	Texture tex;
	tex.filepath = s.c_str();
	//Generate Texture on GPU
	glGenTextures(1, &tex.id); //maybe set multiple textures in a larger buffer in future
	glActiveTexture(textureSlots[slot]); //Different drivers have different numbers of texture slots, so past a certain number it might be helpful to have a system where texture slots are dynamically filled and emptied depending on need
	glBindTexture(GL_TEXTURE_2D, tex.id);
	//Set Texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT); //wrap texture around horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //wrap texture vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int w;
	int h;
	int channels;
	stbi_uc* image = stbi_load(tex.filepath, &w, &h, &channels, 0);
	if (image != nullptr) {
		tex.width = w;
		tex.height = h;
		//Upload image to GPU. GL_RGBA appears twice for internal and external format
		//Check for png, then upload accordingly
		if (s.substr(s.length() - size_t(3), 3) == "png") {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
				0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h,
				0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}
	}
	else {
		printf("Error could not load image '%s'", tex.filepath);
	}
	stbi_image_free(image);
	return tex;
}

void unbindTexture(Texture* tex)
{
	glBindTexture(GL_TEXTURE_2D, tex->id);
}

void uploadTexture(uint32_t id, const char* name, uint32_t tex_slot)
{
	uint32_t loc = glGetUniformLocation(id, name);
	//Doesn't need to take in texture. It knows to find it at slot 0..1, etc
	glUniform1i(loc, tex_slot);
}




