#pragma once
#include "core.h"
#include "Helper.h"

uint32_t compileShader(std::string vSrc, std::string fSrc);
void bindShader(uint32_t programId);
