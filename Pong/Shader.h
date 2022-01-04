#pragma once
#include "core.h"
#include "Helper.h"

class ShaderInfo {
public:
	uint32_t id;
	uint32_t projLoc;
	uint32_t viewLoc;
	uint32_t transLoc;

	ShaderInfo() {
		id = 0;
		projLoc = 0;
		viewLoc = 0;
		transLoc = 0;
	}
};

class ShaderInfoUV : public ShaderInfo {
public:
	uint32_t uvLoc;
	ShaderInfoUV() {
		uvLoc = 0;
	}
};

uint32_t compileShader(std::string vSrc, std::string fSrc);
void bindShader(uint32_t programId);
void deleteShader(uint32_t programId);
