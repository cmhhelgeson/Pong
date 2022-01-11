#include "Graphics.h"

/*void glVertexAttribPointer
* GLuint index, (layout location in shader you want to use)
* GLuint size, (3 for vec3, 4 for vec4)
* GLenum type, (GL_fLOAT, GL_INT, etc)
* GLboolean normalized, (is data normalized, int to float)
* GLsizei stride, (how many bytes each vertex is, sizeof(float)*3 + sizeof(float) * 4
* const void* offsetPointer (asks for offset of attribute withint thed tata
)*/


void glDefineVertex() {
	//Define how each element of the vertice is read in
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
}

void glDefineVertexUV() {
	//Define how each element of the vertice is read in
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, uv));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, texId));
	glEnableVertexAttribArray(3);

}
void createBasicVAO(uint32_t* vao) {
	glCreateVertexArrays(1, vao);
	glBindVertexArray((*vao));
}

void createArrayBuffer(uint32_t* vbo, std::vector<Vertex>* mesh) {
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbo));
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (*mesh).size(), (*mesh).data(), GL_STATIC_DRAW);
}

void createArrayBufferUV(uint32_t* vbo, std::vector<VertexUV> *mesh) {
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbo));
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUV) * (*mesh).size(), (*mesh).data(), GL_STATIC_DRAW);
}

void createArrayBufferMass(uint32_t* vbo, uint32_t num_verts) {
	glCreateBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbo));
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUV) * num_verts, nullptr, GL_DYNAMIC_DRAW); //dynamically populating vertex buffer
}

void createElementBuffer(uint32_t* ebo) {
	glGenBuffers(1, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*ebo));
}

void setupShape(uint32_t* vao, uint32_t* vbo, uint32_t* ebo, std::vector<Vertex>* mesh) {
	createBasicVAO(vao);
	createArrayBuffer(vbo, mesh);
	glDefineVertex();
	createElementBuffer(ebo);
} 

void setupShapeUV(uint32_t* vao, uint32_t* vbo, uint32_t* ebo, std::vector<VertexUV>* mesh) {
	createBasicVAO(vao);
	createArrayBufferUV(vbo, mesh);
	glDefineVertexUV();
	createElementBuffer(ebo);
}

void setupShapeMass(uint32_t* vao, uint32_t* vbo, uint32_t* ebo) {
	createBasicVAO(vao);
	createArrayBufferMass(vbo, 100);
	glDefineVertexUV();
	createElementBuffer(ebo);

}

void destroyGlContext(glContext* context) {
	glDeleteBuffers(1, &(context->vao));
	glDeleteBuffers(1, &(context->ebo));
	glDeleteVertexArrays(1, &(context->vao));
}

