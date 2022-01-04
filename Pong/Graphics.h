#pragma once
#include "core.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec4 color;
};

struct VertexUV {
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 uv;
};

struct glContext {
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;
}; 

void glDefineVertex();
void glDefineVertexUV();
void createBasicVAO(uint32_t* vao);
void createArrayBuffer(uint32_t* vbo, std::vector<Vertex>* mesh);
void createArrayBufferUV(uint32_t* vbo, std::vector<VertexUV>* mesh);
void createElementBuffer(uint32_t* ebo);

void setupShape(uint32_t* vao, uint32_t* vbo, uint32_t* ebo, std::vector<Vertex>* mesh);

void setupShapeUV(uint32_t* vao, uint32_t* vbo, uint32_t* ebo, std::vector<VertexUV>* mesh);
void destroyGlContext(glContext* context);