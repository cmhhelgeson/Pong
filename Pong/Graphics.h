#pragma once
#include "core.h"
#include "Texture.h"

/*VERTEX AND OPENGL INITIALIZATION*/

struct Vertex {
	glm::vec3 pos;
	glm::vec4 color;
};

struct VertexUV {
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 uv;
	float texId;
};

struct glContext {
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;
}; 

//Define Buffer Location of struct elements
void glDefineVertex();
void glDefineVertexUV();
//VAO
void createBasicVAO(uint32_t* vao);
//ARRAY BUFFER STATIC DRAW
void createArrayBuffer(uint32_t* vbo, std::vector<Vertex>* mesh);
void createArrayBufferUV(uint32_t* vbo, std::vector<VertexUV>* mesh);
//ARRAY BUFFER DYNAMIC DRAW
void createArrayBufferMass(uint32_t* vbo, uint32_t num_verts);
//ELEMENT BUFFER
void createElementBuffer(uint32_t* ebo);
//CREATE SHAPE STATIC
void setupShape(uint32_t* vao, uint32_t* vbo, uint32_t* ebo, std::vector<Vertex>* mesh);
void setupShapeUV(uint32_t* vao, uint32_t* vbo, uint32_t* ebo, std::vector<VertexUV>* mesh);
//CREATE SHAPE DYNAMIC
void setupShapeMass(uint32_t* vao, uint32_t* vbo, uint32_t* ebo);

void destroyGlContext(glContext* context);

/*SPRITES AND SPRITESHEETS*/

struct Sprite {
	Texture* texture;
	glm::vec2 texCoords[4];
	Sprite() = default;
	Sprite(Texture* _texture);
	Sprite(Texture* _texture, glm::vec2 _coords);
	Sprite(Texture* _texture, glm::vec2* _texCoords);
};

struct SpriteSheet {
	Texture* texture;
	int nSprites;
	std::vector<Sprite> sprites;
	//Sprite* sprites;
	SpriteSheet() = default;
	SpriteSheet(Texture* _texture, int spr_w, int spr_h, int numSprites, int spacing);
	~SpriteSheet();
};