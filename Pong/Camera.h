#pragma once
#include "core.h"
struct Camera {
	glm::mat4x4 projMat;
	glm::mat4x4 viewMat;
	glm::vec2 pos;
};

void adjustProjection(Camera* cam);

void initViewMatrix(Camera* cam);

Camera* initCamera(glm::vec2 _pos);