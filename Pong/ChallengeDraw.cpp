//Challenge 1 Draw a Triangle
/* static std::vector<Vertex> triangleArray = {
	Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f)},
	Vertex{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f)},
	Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f)}
};

static uint32_t triangleElements[3] = { 0, 1, 2 };

static glContext Triangle;

void setUpTriangle() {
	setupShape(&Triangle.vao, &Triangle.vbo, &Triangle.ebo, &triangleArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleElements), triangleElements, GL_STATIC_DRAW);
}

void drawTriangle() {
	glBindVertexArray(Triangle.vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void drawTriangleElements() {
	glBindVertexArray(Triangle.vao);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void destroyTriangle() {
	destroyGlContext(&Triangle);
}

void setupChallenge2() {
	setupShapeUV(&Square.vao, &Square.vbo, &Square.ebo, &squareArrayUV);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(challenge2Elements), challenge2Elements, GL_STATIC_DRAW);
}

void drawChallenge2() {
	glBindVertexArray(Square.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroyChallenge2() {
	destroyGlContext(&Square);
}

static uint32_t challenge3Elements[24] = {
	0, 1, 9,     1, 2, 3,     3, 4, 5,
	5, 6, 7,     7, 8, 9,     5, 7, 9,
	9, 1, 3,     3, 5, 9
};


static std::vector<Vertex> challenge3Star = {
	//      Color                         Position
	Vertex{glm::vec3(-0.4f,  0.125f, 0.0f)   , glm::vec4(0.4f, 0.521f, 0.960f, 1.0f)},
	Vertex{glm::vec3(-0.125f,  0.125f, 0.0f) , glm::vec4(0.490f, 0.443f, 0.956f, 1.0f)},
	Vertex{glm::vec3(0.0f,    0.5f, 0.0f)    , glm::vec4(0.686f, 0.443f, 0.956f, 1.0f)},
	Vertex{glm::vec3(0.125f,  0.125f, 0.0f)  , glm::vec4(0.917f, 0.443f, 0.956f, 1.0f)},
	Vertex{glm::vec3(0.4f,  0.125f, 0.0f)    , glm::vec4(0.807f, 0.317f, 0.250f, 1.0f)},
	Vertex{glm::vec3(0.13f, -0.125f, 0.0f)   , glm::vec4(0.807f, 0.250f, 0.682f, 1.0f)},
	Vertex{glm::vec3(0.29f,   -0.6f, 0.0f)   , glm::vec4(0.956f, 0.631f, 0.443f, 1.0f)},
	Vertex{glm::vec3(0.0f,  -0.29f, 0.0f)    , glm::vec4(0.956f, 0.843f, 0.443f, 1.0f)},
	Vertex{glm::vec3(-0.29f,   -0.6f, 0.0f)  , glm::vec4(0.862f, 0.956f, 0.443f, 1.0f)},
	Vertex{glm::vec3(-0.13f, -0.125f, 0.0f)  , glm::vec4(0.584f, 0.956f, 0.443f, 1.0f)}
};

static glContext Star;

void setupChallenge3() {
	setupShape(&Star.vao, &Star.vbo, &Star.ebo, &challenge3Star);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(challenge3Elements), challenge3Elements, GL_STATIC_DRAW);
}

void drawChallenge3() {
	glBindVertexArray(Star.vao);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
}

void destroyChallenge3() {
	destroyGlContext(&Star);
}

static std::vector<Vertex> challenge4Square = {
	Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.11f, 0.8f, 0.76f, 1.0f)},
	Vertex{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec4(0.1f,  0.9f, 0.12f, 1.0f)},
	Vertex{glm::vec3(0.5f,  0.5f, 0.0f), glm::vec4(0.12f, 0.9f, 0.1f,  1.0f)},
	Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.12f, 0.1f, 0.9f,  1.0f)}
};

static uint32_t challenge4Elements[8] = {
	0, 1,
	1, 2,
	2, 3,
	3, 0,
};

static glContext lineSquare;

void setupChallenge4() {
	setupShape(&lineSquare.vao, &lineSquare.vbo, &lineSquare.ebo, &challenge4Square);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(challenge4Elements), challenge4Elements, GL_STATIC_DRAW);
}

void drawChallenge4() {
	glBindVertexArray(lineSquare.vao);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
}

void destroyChallenge4() {
	destroyGlContext(&lineSquare);
}

static glContext cube;
static std::vector<Vertex> cubeArray = {
	Vertex{glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f,-1.0f),  glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f, 1.0f),	 glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f,-1.0f),	 glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f,-1.0f),	 glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f,-1.0f),	 glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f, 1.0f),	 glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f,-1.0f),	glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f,-1.0f),	glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f,-1.0f),	glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f, 1.0f),	glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f,-1.0f),	glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f, 1.0f),	 glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f, 1.0f),	 glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)},
	Vertex{glm::vec3(1.0f,-1.0f, 1.0f),	 glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)}
};

void setUpCube() {
	setupShape(&cube.vao, &cube.vbo, &cube.ebo, &cubeArray);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleElements), triangleElements, GL_STATIC_DRAW);
}

void drawCube() {
	glBindVertexArray(cube.vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void destroyCube() {
	destroyGlContext(&cube);
}

void destroyChallenge() {
	switch (currentChallenge) {
	case 1: {
		destroyTriangle();
		break;
	}
	case 2: {
		destroyTriangle();
		break;
	} case 3: {
		destroyChallenge2();
		break;
	} case 4: {
		destroyChallenge3();
		break;
	}
	case 5: {
		destroyChallenge4();
		break;
	}
	case 6: {
		destroyCube();
		break;
	}
	default: {
		break;
	}
	}
}

void setupChallenge() {
	if (Key::isKeyDown(GLFW_KEY_1)) {
		if (currentChallenge != 1) {
			destroyChallenge();
			setUpTriangle();
			currentChallenge = 1;
		}
	}
	else if (Key::isKeyDown(GLFW_KEY_2)) {
		if (currentChallenge != 2) {
			destroyChallenge();
			setUpTriangle();
			currentChallenge = 2;
		}
	}
	else if (Key::isKeyDown(GLFW_KEY_3)) {
		if (currentChallenge != 3) {
			destroyChallenge();
			setupChallenge2();
			currentChallenge = 3;
		}
	}
	else if (Key::isKeyDown(GLFW_KEY_4)) {
		if (currentChallenge != 4) {
			destroyChallenge();
			setupChallenge3();
			currentChallenge = 4;
		}
	}
	else if (Key::isKeyDown(GLFW_KEY_5)) {
		if (currentChallenge != 5) {
			destroyChallenge();
			setupChallenge4();
			currentChallenge = 5;
		}
	}
	else if (Key::isKeyDown(GLFW_KEY_6)) {
		if (currentChallenge != 6) {
			destroyChallenge();
			setUpCube();
			currentChallenge = 6;
		}
	}
}

void drawChallenge() {
	switch (currentChallenge) {
	case 1: {
		drawTriangle();
		break;
	}
	case 2: {
		drawTriangleElements();
		break;
	}
	case 3: {
		drawChallenge2();
		break;
	}
	case 4: {
		drawChallenge3();
		break;
	}
	case 5: {
		drawChallenge4();
		break;
	}
	case 6: {
		drawCube();
	}
	default: {
		break;
	}
	}
} */