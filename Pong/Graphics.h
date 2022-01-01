#pragma once
#include "core.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec4 color;
};

void glDefineVertex();
void createBasicVAO(uint32_t* vao);
void createArrayBuffer(uint32_t* vbo, std::vector<Vertex>* mesh);
void createElementBuffer(uint32_t* ebo);
void setupShape(uint32_t* vao, uint32_t* vbo, uint32_t* ebo, std::vector<Vertex>* mesh);