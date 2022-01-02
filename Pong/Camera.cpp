#include "Camera.h"

void adjustProjection(Camera* cam) {
	cam->projMat = glm::mat4(1.0f);
	cam->projMat =  glm::ortho(0.0f, 32.0f * 40.0f, 0.0f, 32.0f * 21.0f, 0.0f, 100.0f);
}

void initViewMatrix(Camera* cam) {
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cam->viewMat = glm::mat4(1.0f);
	cam->viewMat = cam->viewMat * glm::lookAt(
		glm::vec3(cam->pos.x, cam->pos.y, 20.0f),
		glm::vec3(cameraFront.x + cam->pos.x, cameraFront.y + cam->pos.y, 0.0f),
		cameraUp);
}

Camera* initCamera(glm::vec2 _pos) {
	Camera *cam = new Camera();
	cam->pos = _pos;
	adjustProjection(cam);
	initViewMatrix(cam);
	return cam;
}