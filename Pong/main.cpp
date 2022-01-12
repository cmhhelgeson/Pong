#include "Window.h"
#include "Shader.h"
#include "Graphics.h"
#include "Camera.h"
#include <entt/entt.hpp>
#include <entt/core/type_traits.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TAU (M_PI * 2)

struct Scene {
	glm::vec3 backColor;
	bool changingScene = false;
	float fadeTime = 2.0f;
};

struct Texture {
	const char* filepath;
	uint32_t id;
};

struct TransformComponent {
	glm::mat4 transform;
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::mat4& _transform) : transform(_transform) {

	}
	operator const glm::mat4& () { return transform; }
};

static GLenum textureSlots[6] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5};

Texture* initTexture(std::string s, int slot) {
	Texture* tex = new Texture();
	tex->filepath = s.c_str();
	//Generate Texture on GPU
	glGenTextures(1, &tex->id); //maybe set multiple textures in a larger buffer in future
	glActiveTexture(textureSlots[slot]); //Different drivers have different numbers of texture slots, so past a certain number it might be helpful to have a system where texture slots are dynamically filled and emptied depending on need
	glBindTexture(GL_TEXTURE_2D, tex->id);
	//Set Texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT); //wrap texture around horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //wrap texture vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int w;
	int h;
	int channels;
	stbi_uc *image = stbi_load(tex->filepath, &w, &h, &channels, 0);
	if (image != nullptr) {
		//Upload image to GPU. GL_RGBA appears twice for internal and external format
		//Check for png, then upload accordingly
		if (s.substr(s.length() - size_t(3), 3) == "png") {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
				0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h,
				0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}	
	} else {
		printf("Error could not load image '%s'", tex->filepath);
	}
	stbi_image_free(image);
	return tex;
}

void bindTexture(Texture *tex) {
	glBindTexture(GL_TEXTURE_2D, tex->id);
}

void unbindTexture(Texture* tex) {
	glBindTexture(GL_TEXTURE_2D, tex->id);
}

void uploadTexture(uint32_t id, const char* name, uint32_t tex_slot) {
	uint32_t loc = glGetUniformLocation(id, name);
	//Doesn't need to take in texture. It knows to find it at slot 0..1, etc
	glUniform1i(loc, tex_slot);
}


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

static glContext Square;

std::vector<Vertex> createSquare(float start, float width, float height) {
	return {
		Vertex{glm::vec3(start, start, 0.0f) , glm::vec4(0.11f, 0.8f, 0.76f, 1.0f)},
		Vertex{glm::vec3(start,  start + height, 0.0f) , glm::vec4(0.1f,  0.9f, 0.12f, 1.0f)},
		Vertex{glm::vec3(start + width,  start + height, 0.0f) ,  glm::vec4(0.12f, 0.9f, 0.1f,  1.0f)},
		Vertex{glm::vec3(start + width, start, 0.0f) , glm::vec4(0.12f, 0.1f, 0.9f,  1.0f)}
	};
}

//Two Quads in a single draw call
std::vector<VertexUV> createSquareUV(float start, float width, float height, float tex_scale_w, float tex_scale_h) {
	/* return {
		VertexUV{glm::vec3(start, start, 0.0f) , glm::vec4(0.11f, 0.8f, 0.76f, 1.0f), glm::vec2(tex_scale_w, tex_scale_h)},           
		VertexUV{glm::vec3(start,  start + height, 0.0f) , glm::vec4(0.1f,  0.9f, 0.12f, 1.0f), glm::vec2(tex_scale_w, 0.0f) },
		VertexUV{glm::vec3(start + width,  start + height, 0.0f) ,  glm::vec4(0.12f, 0.9f, 0.1f,  1.0f), glm::vec2(0.0f, 0.0f)},
		VertexUV{glm::vec3(start + width, start, 0.0f) , glm::vec4(0.12f, 0.1f, 0.9f,  1.0f), glm::vec2(0.0f, tex_scale_h)}, 

		VertexUV{glm::vec3(start + 400.0f, start, 0.0f) , glm::vec4(0.11f, 0.8f, 0.76f, 1.0f), glm::vec2(tex_scale_w, tex_scale_h)},
		VertexUV{glm::vec3(start + 400.0f,  start + height, 0.0f) , glm::vec4(0.1f,  0.9f, 0.12f, 1.0f), glm::vec2(tex_scale_w, 0.0f) },
		VertexUV{glm::vec3(start + 400.0f + width,  start + height, 0.0f) ,  glm::vec4(0.12f, 0.9f, 0.1f,  1.0f), glm::vec2(0.0f, 0.0f)},
		VertexUV{glm::vec3(start + 400.0f + width, start, 0.0f) , glm::vec4(0.12f, 0.1f, 0.9f,  1.0f), glm::vec2(0.0f, tex_scale_h)}
	}; */

	return {
		VertexUV{glm::vec3(start, start, 0.0f) , glm::vec4(0.11f, 0.8f, 0.76f, 1.0f), glm::vec2(tex_scale_w, tex_scale_h), 0.0f},
		VertexUV{glm::vec3(start,  start + height, 0.0f) , glm::vec4(0.1f,  0.9f, 0.12f, 1.0f), glm::vec2(tex_scale_w, 0.0f), 0.0f},
		VertexUV{glm::vec3(start + width,  start + height, 0.0f) ,  glm::vec4(0.12f, 0.9f, 0.1f,  1.0f), glm::vec2(0.0f, 0.0f), 0.0f},
		VertexUV{glm::vec3(start + width, start, 0.0f) , glm::vec4(0.12f, 0.1f, 0.9f,  1.0f), glm::vec2(0.0f, tex_scale_h), 0.0f},

		VertexUV{glm::vec3(start + 400.0f, start, 0.0f) , glm::vec4(0.11f, 0.8f, 0.76f, 1.0f), glm::vec2(tex_scale_w, tex_scale_h), 1.0f},
		VertexUV{glm::vec3(start + 400.0f,  start + height, 0.0f) , glm::vec4(0.1f,  0.9f, 0.12f, 1.0f), glm::vec2(tex_scale_w, 0.0f), 1.0f},
		VertexUV{glm::vec3(start + 400.0f + width,  start + height, 0.0f) ,  glm::vec4(0.12f, 0.9f, 0.1f,  1.0f), glm::vec2(0.0f, 0.0f), 1.0f},
		VertexUV{glm::vec3(start + 400.0f + width, start, 0.0f) , glm::vec4(0.12f, 0.1f, 0.9f,  1.0f), glm::vec2(0.0f, tex_scale_h), 1.0f}
	};
}

static uint32_t challenge2Elements[12] = {
	0, 1, 2, 0, 2, 3, 
	4, 5, 6, 6, 7, 4
};

//STATIC DRAW BATCH RENDERING
void setupChallenge2() {
	std::vector<VertexUV> vec = createSquareUV(0.0f, 300.0f, 300.f, 1.0f, 1.0f);
	setupShapeUV(&Square.vao, &Square.vbo, &Square.ebo, &vec);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(challenge2Elements), challenge2Elements, GL_STATIC_DRAW);
} 


void drawChallenge2() {
	glBindVertexArray(Square.vao);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

void destroyChallenge2() {
	destroyGlContext(&Square);
}

struct AssetPool {
	std::unordered_map<std::string, ShaderInfo> shaders;
	std::unordered_map<std::string, Texture> textures;
};

ShaderInfo *GetShaderFromPool(AssetPool* pool, std::string vertSrc, std::string fragSrc) {
	std::unordered_map<std::string, ShaderInfo>::iterator it = pool->shaders.find(vertSrc);
	if (it != pool->shaders.end()) {
		return &(it->second);
	} else {
		ShaderInfo shader; 
		shader.id = compileShader(vertSrc, fragSrc);
		pool->shaders.insert({ vertSrc, shader });
		it = pool->shaders.find(vertSrc);
		return &(it->second);
	}
}

Texture *GetTextureFromPool(AssetPool* pool, std::string fileName) {
	std::unordered_map<std::string, Texture>::iterator it = pool->textures.find(fileName);
	if (pool->textures.find(fileName) != pool->textures.end()) {
		return &(it->second);
	}
	else {
		Texture *texture = initTexture(fileName, pool->textures.size() - 1);
		return texture; 
	}
}

static int currentChallenge = 0;

void update(double dt, entt::registry& registry) {
	auto view = registry.view<TransformComponent>();
	float deltaX = 0.0f;
	float deltaY = 0.0f;
	if (Key::isKeyHeld(GLFW_KEY_LEFT)) {
		deltaX = -100.0f * dt;
	}
	if (Key::isKeyHeld(GLFW_KEY_RIGHT)) {
		deltaX = 100.0f * dt;
	}
	if (Key::isKeyHeld(GLFW_KEY_UP)) {
		deltaY = 100.0f * dt;
	}
	if (Key::isKeyHeld(GLFW_KEY_DOWN)) {
		deltaY = -100.0f * dt;
	}

	

	for (auto entity : view) {
		TransformComponent& transform = view.get<TransformComponent>(entity);
	}

}

uint32_t* generateQuadIndices(int num_quads) {
	uint32_t* verts = (uint32_t*)malloc(num_quads * 6 * sizeof(uint32_t));

	for (int i = 0; i < num_quads; i++) {
		(*verts) = 0 + i * 4; //verts[0]
		verts++;

		(*verts) = 1 + i * 4; //verts[1]
		verts++;

		(*verts) = 2 + i * 4; //verts[2]
		verts++;

		(*verts) = 2 + i * 4; //verts[3]
		verts++;

		(*verts) = 3 + i * 4; //verts[4]
		verts++;

		(*verts) = 0 + i * 4; //verts[5]
		verts++;
	}
	return verts;

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

	glfwSetKeyCallback(win->window, cmh_key_callback);
	glfwSetMouseButtonCallback(win->window, cmh_mouse_button_callback);
	glfwSetFramebufferSizeCallback(win->window, cmh_resize_callback);

	ShaderInfo s2;
	s2.id = compileShader("default_uv_vert.glsl", "default_uv_frag.glsl");
	Texture* tex1 = initTexture(std::string("assets/TestImage2.png"), 0);
	Texture* tex2 = initTexture(std::string("assets/wall.jpg"), 1);
	glViewport(0, 0, win->sizeX, win->sizeY);
	useShader(s2.id);
	s2.transLoc = glGetUniformLocation(s2.id, "transform");
	s2.viewLoc = (glGetUniformLocation(s2.id, "view"));
	s2.projLoc = (glGetUniformLocation(s2.id, "proj"));
	auto texturesLoc = glGetUniformLocation(s2.id, "uTextures");
	int samplers[2] = { 0, 1 };

	//entt practice
	entt::registry registry;
	//Entity uint32_t identifier
	entt::entity entity = registry.create();
	//Create component. Identifier + any args to be passed into the component
	registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

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
		if (currentChallenge == 0) {
			setupChallenge2();
			currentChallenge = 2;
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
			deltaX = -100.0f * dt;
		}
		if (Key::isKeyHeld(GLFW_KEY_RIGHT) ) {
			deltaX = 100.0f * dt;
		}
		if (Key::isKeyHeld(GLFW_KEY_UP)) {
			deltaY = 100.0f * dt;
		}
		if (Key::isKeyHeld(GLFW_KEY_DOWN)) {
			deltaY = -100.0f * dt;
		} 
		glClearColor(curScene->backColor.r, curScene->backColor.g, curScene->backColor.b, 1.0f);
		//Clear Screen for next draw
		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(s2.projLoc, 1, GL_FALSE, glm::value_ptr(cam->projMat));
		glUniformMatrix4fv(s2.viewLoc, 1, GL_FALSE, glm::value_ptr(cam->viewMat));
		auto& trans = registry.get<TransformComponent>(entity);
		trans.transform = glm::translate(trans.transform, glm::vec3(deltaX, deltaY, 0.0f));
		glUniformMatrix4fv(s2.transLoc, 1, GL_FALSE, glm::value_ptr(trans.transform));

		glUniform1iv(texturesLoc, 2, samplers);
		deltaY = 0.0f;
		deltaX = 0.0f;
		drawChallenge2();

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

	destroyChallenge2();
	deleteShader(s2.id);
	unbindTexture(tex1);
	//unbindTexture(tex2);
	free(tex1);
	free(curScene);
	free(cam);

	deleteWindow(win);

	glfwTerminate();
	return 0;


}