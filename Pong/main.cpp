#include "Window.h"
#include "Shader.h"
#include "Graphics.h"
#define TAU (M_PI * 2)

struct Camera {
	glm::mat4x4 projMat;
	glm::mat4x4 viewMat;
	glm::vec2 pos;
};

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

struct Scene {
	glm::vec3 backColor;
	bool changingScene = false;
	float fadeTime = 2.0f;
};

namespace Input {
	namespace Key {
		bool keyIsPressed[GLFW_KEY_LAST];
		std::array<bool, GLFW_KEY_LAST> keyCurState = { 0 };
		std::array<bool, GLFW_KEY_LAST> keyPrevState = { 0 };
		bool isKeyDown(int key) {
			if (key >= 0 && key < GLFW_KEY_LAST) {
				return keyIsPressed[key];
			}
		}
		bool isKeyHeld(int key) {
			if (key >= 0 && key < GLFW_KEY_LAST) {
				return (keyCurState[key] == keyPrevState[key] && keyCurState[key] == GLFW_PRESS);
			}
		}	
	}
	namespace Mouse {
		bool mouseIsPressed[GLFW_MOUSE_BUTTON_LAST];
	}
};

using namespace Input;


void cmh_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key >= 0 && key < GLFW_KEY_LAST) {
		Key::keyIsPressed[key] = action == GLFW_PRESS;
		std::cout << "Prev State: " << Key::keyPrevState[key] << " Current State: " << Key::keyCurState[key] << '\n'; 
		if (action != GLFW_REPEAT) {
			Key::keyCurState[key] = action;
		} 
	}
}

void cmh_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
		Mouse::mouseIsPressed[button] = action == GLFW_PRESS;
	}
}

void cmh_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


Scene* initScene(float r, float g, float b) {
	Scene* s = new Scene();
	s->backColor = glm::vec3(r, g, b);
	return s;
}


Scene* changeScene(Scene* cur_scene) {
	free(cur_scene);
	return initScene(42.0f / 255.0f, 252.0f / 255.0f, 3.0f / 255.0f);
}

void updateScene(double dt, Scene** cur_scene) {
	if (!(*cur_scene)->changingScene && Key::keyIsPressed[GLFW_KEY_P]) {
		(*cur_scene)->changingScene = true;
		printf("Changing scene\n");
	}
	if ((*cur_scene)->changingScene && (*cur_scene)->fadeTime > 0.0f) {
		(*cur_scene)->fadeTime -= dt * 2.0f;
		(*cur_scene)->backColor.r -= dt * 2.0f;
		(*cur_scene)->backColor.b -= dt * 2.0f;
		(*cur_scene)->backColor.g -= dt * 2.0f;
	} else if ((*cur_scene)->changingScene) {
		(*cur_scene) = changeScene(*cur_scene);
		(*cur_scene)->changingScene = false;
	}

}

//Challenge 1 Draw a Triangle
static std::vector<Vertex> triangleArray = {
	Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f)},
	Vertex{glm::vec3(0.0f, 0.5f, 0.0f ), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f)},
	Vertex{glm::vec3(0.5f, -0.5f, 0.0f ), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f)}
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

static glContext Square;

/*static std::vector<Vertex> squareArray = {
	Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.11f, 0.8f, 0.76f, 1.0f)},
	Vertex{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec4(0.1f,  0.9f, 0.12f, 1.0f)},
	Vertex{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec4(0.12f, 0.9f, 0.1f,  1.0f)},
	Vertex{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec4(0.12f, 0.1f, 0.9f,  1.0f)}
};  */

static std::vector<Vertex> squareArray = {
	Vertex{glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec4(0.11f, 0.8f, 0.76f, 1.0f)},
	Vertex{glm::vec3(0.0f,  300.0f, 0.0f) , glm::vec4(0.1f,  0.9f, 0.12f, 1.0f)},
	Vertex{glm::vec3(300.0f,  300.0f, 0.0f) ,  glm::vec4(0.12f, 0.9f, 0.1f,  1.0f)},
	Vertex{glm::vec3( 300.0f, 0.0f, 0.0f) , glm::vec4(0.12f, 0.1f, 0.9f,  1.0f)}
}; 

static uint32_t challenge2Elements[6] = {
	0, 1, 2,
	0, 2, 3
};

void setupChallenge2() {
	setupShape(&Square.vao, &Square.vbo, &Square.ebo, &squareArray);
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
	Vertex{glm::vec3(-0.125f,  0.125f, 0.0f) , glm::vec4( 0.490f, 0.443f, 0.956f, 1.0f )},
	Vertex{glm::vec3(0.0f,    0.5f, 0.0f)    , glm::vec4( 0.686f, 0.443f, 0.956f, 1.0f )},   
	Vertex{glm::vec3(0.125f,  0.125f, 0.0f)  , glm::vec4( 0.917f, 0.443f, 0.956f, 1.0f )}, 
	Vertex{glm::vec3(0.4f,  0.125f, 0.0f)    , glm::vec4( 0.807f, 0.317f, 0.250f, 1.0f )},   
	Vertex{glm::vec3(0.13f, -0.125f, 0.0f)   , glm::vec4( 0.807f, 0.250f, 0.682f, 1.0f )},  
	Vertex{glm::vec3(0.29f,   -0.6f, 0.0f)   , glm::vec4( 0.956f, 0.631f, 0.443f, 1.0f )},  
	Vertex{glm::vec3(0.0f,  -0.29f, 0.0f)    , glm::vec4( 0.956f, 0.843f, 0.443f, 1.0f )},   
	Vertex{glm::vec3(-0.29f,   -0.6f, 0.0f)  , glm::vec4( 0.862f, 0.956f, 0.443f, 1.0f )}, 
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
	setupShape(&lineSquare.vao , &lineSquare.vbo, &lineSquare.ebo, &challenge4Square);
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

static int currentChallenge = 0;

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
}

int main() {
	if (!glfwInit()) {
		printf("Failed to initialize GLFW");
		return -1;
	}

	double timeStarted = glfwGetTime();

	Window* win = initializeWindow(1280, 720, "Pong");
	if (win == nullptr) {
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		glfwTerminate();
		return -1;
	}

	uint32_t shaderId = compileShader("default_vert.glsl", "default_frag.glsl");

	glfwSetKeyCallback(win->window, cmh_key_callback);
	glfwSetMouseButtonCallback(win->window, cmh_mouse_button_callback);
	glfwSetFramebufferSizeCallback(win->window, cmh_resize_callback);
	//Which pixels to draw to glViewport(lower left corner, width, height)
	glViewport(0, 0, win->sizeX, win->sizeY);
	
	bindShader(shaderId);
	uint32_t transformLoc = glGetUniformLocation(shaderId, "transform");
	glm::mat4 trans(1.0f);
	uint32_t viewLoc = glGetUniformLocation(shaderId, "view");
	uint32_t projLoc = glGetUniformLocation(shaderId, "proj");

	Camera* cam = initCamera(glm::vec2(0.0f, 0.0f));

	Scene* curScene = initScene(250.0f / 255.0f, 119.0f/255.0f, 110.0f/255.0f);
	static double fps = 1.0 / 60.0;
	double beginFrameTime = glfwGetTime(), beginSecondTime = glfwGetTime();
	double endFrameTime = glfwGetTime(), endSecondTime = glfwGetTime();
	double dt = -1.0f;

	currentChallenge = 0;

	float deltaX = 0.0f;
	float deltaY = 0.0f;
	
	int frameCount = 0;
	while (!glfwWindowShouldClose(win->window)) {
		Key::keyPrevState = Key::keyCurState;
		//Setup Draw
		if (Key::isKeyDown(GLFW_KEY_1)) {
			if (currentChallenge != 1) {
				destroyChallenge();
				setUpTriangle();
				currentChallenge = 1;
			}
		} else if (Key::isKeyDown(GLFW_KEY_2)) {
			if (currentChallenge != 2) {
				destroyChallenge();
				setUpTriangle();
				currentChallenge = 2;
			}
		} else if (Key::isKeyDown(GLFW_KEY_3)) {
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
		updateScene(dt, &curScene);
		if (Key::keyIsPressed[GLFW_KEY_F]) {
			toggleFullScreen(win, true);
		}
		if (Key::keyIsPressed[GLFW_KEY_P]) {
			updateScene(dt, &curScene);
			printf("Changed Scene\n");
		}
		if (Key::keyIsPressed[GLFW_KEY_M]) {
			toggleFullScreen(win, false);
		}
		if (Key::keyIsPressed[GLFW_KEY_ESCAPE]) {
			glfwSetWindowShouldClose(win->window, true);
		}
		if (Key::isKeyHeld(GLFW_KEY_LEFT) ){
			deltaX = -100.0f;
		}
		if (Key::isKeyHeld(GLFW_KEY_RIGHT) ) {
			deltaX = 100.0f;
		}
		if (Key::isKeyHeld(GLFW_KEY_UP)) {
			deltaY = 100.0f;
		}
		if (Key::isKeyHeld(GLFW_KEY_DOWN)) {
			deltaY = -100.0f;
		} 
		glClearColor(curScene->backColor.r, curScene->backColor.g, curScene->backColor.b, 1.0f);
		//Clear Screen for next draw
		glClear(GL_COLOR_BUFFER_BIT);

		trans = glm::translate(trans, glm::vec3(deltaX * dt, deltaY * dt, 0.0f));
		//trans = glm::rotate(trans, glm::radians(0.1f), glm::vec3(0.0, 0.0, 1.0));;
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam->projMat));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->viewMat));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		deltaY = 0.0f;
		deltaX = 0.0f;
		drawChallenge();

		//Double Buffering
		glfwSwapBuffers(win->window);
		glfwPollEvents();
		
		endFrameTime = glfwGetTime() , endSecondTime = glfwGetTime();
		dt = endFrameTime - beginFrameTime;
		frameCount++;
		if (endSecondTime - beginSecondTime >= 1.0) {
			printf("%dFPS\n", frameCount);
			frameCount = 0;
			beginSecondTime = endSecondTime;
		}
		beginFrameTime = endFrameTime;
	}

	free(curScene);
	free(cam);

	deleteWindow(win);

	glfwTerminate();
	return 0;


}