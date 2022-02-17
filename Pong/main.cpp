#include "Window.h"
#include "Shader.h"
#include "Graphics.h"
#include "Camera.h"
#include "Keyboard.h"
#include <entt/entt.hpp>
#include <entt/core/type_traits.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> 
#include "C:/jsoncpp/dist/json/json.h"
#define TAU (M_PI * 2)



struct Scene {
	glm::vec3 backColor;
	std::string sceneType;
	std::string assetPath;
	bool changingScene = false;
	float fadeTime = 2.0f;
	void update();
};

Scene* initScene(int newScene) {
	Scene* scene = new Scene();
	std::string filename = "scene_files/scene" + std::to_string(newScene) + ".json";
	std::ifstream scene_file(filename);
	Json::Value root;
	scene_file >> root;
	scene->backColor = 
		glm::vec3(root["color"]["r"].asFloat() / 255.0f, root["color"]["g"].asFloat() / 255.0f, root["color"]["b"].asFloat() / 255.0f);
	scene->sceneType = root["type"].asString();
	scene->assetPath = root["assetPath"].asString();
	scene->fadeTime = root["fadeTime"].asFloat();
	return scene;
}

struct TransformComponent {
	glm::mat4 transform;
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::mat4& _transform) : transform(_transform) {

	}
	operator const glm::mat4& () { return transform; }
};

struct RenderBatch {
	const int pos_size = 2;
	const int color_size = 4;
	const int pos_offset = 0;
	const int color_offset = pos_offset + pos_size * sizeof(float);
	const int vertex_size = 6;
	const int vertex_size_bytes = vertex_size * sizeof(float);

};

struct Velocity2DComponent {
	glm::vec2 velocity;
	Velocity2DComponent() = default;
	Velocity2DComponent(const Velocity2DComponent&) = default;
	Velocity2DComponent(const glm::vec2& _vel) : velocity(_vel) {

	}
	operator const glm::vec2& () { return velocity; }
};

static glm::vec2 standardTextureUV[4] = {
	glm::vec2(1.0f, 1.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(0.0f, 1.0f),
	glm::vec2(0.0f, 0.0f)
};

struct SpriteRendererComponent {
	glm::vec4 color;
	glm::vec2* texCoords;
	Texture* texture;

	SpriteRendererComponent() = default;
	SpriteRendererComponent(const glm::vec4& _color) : color(_color) {
		texture = nullptr;
		texCoords = standardTextureUV;
	}
	SpriteRendererComponent(Texture* _texture): texture(_texture) {
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		texCoords = standardTextureUV;
	}

	SpriteRendererComponent(Texture* _texture, glm::vec2* _coords) :
		texture(_texture), texCoords(_coords) {
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
};

using namespace Input;
Key keyboard;
Mouse mouse;


void cmh_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key >= 0 && key < GLFW_KEY_LAST) {
		//Key::keyIsHeld[key] = (Key::keyIsPressed[key] && action == GLFW_PRESS);
		keyboard.keyIsPressed[key] = action == GLFW_PRESS;
		std::cout << "Prev State: " << keyboard.keyPrevState[key] << " Current State: " << keyboard.keyCurState[key] << '\n'; 
		if (action != GLFW_REPEAT) {
			keyboard.keyCurState[key] = action;
		} 
	}
}

void cmh_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
		mouse.mouseIsPressed[button] = action == GLFW_PRESS;
	}
}

void cmh_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void setCallbacks(Window* win) {
	glfwSetKeyCallback(win->window, cmh_key_callback);
	glfwSetMouseButtonCallback(win->window, cmh_mouse_button_callback);
	glfwSetFramebufferSizeCallback(win->window, cmh_resize_callback);
}


Scene* changeScene(Scene* cur_scene) {
	free(cur_scene);
	return initScene(2);
}

void updateScene(double dt, Scene** cur_scene) {
	if (!(*cur_scene)->changingScene && keyboard.keyIsPressed[GLFW_KEY_P]) {
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

void fillVectorWithQuads(std::vector<VertexUV>& vec, float start, float width, float height, float tex_scale_w, float tex_scale_h, int numRows, int numCols, int numTexIds) {
	float curId = 0.0f;
	float horizontalOffset = 0.0f;
	float verticalOffset = 0.0f;
	for (int y = 0; y < numCols; y++) {
		for (int x = 0; x < numRows; x++) {
			vec.push_back(VertexUV{ glm::vec3(start + horizontalOffset, start + verticalOffset, 0.0f) , glm::vec4(0.11f, 0.8f, 0.76f, 1.0f), glm::vec2(tex_scale_w, tex_scale_h), curId });
			vec.push_back(VertexUV{ glm::vec3(start + horizontalOffset,  start + height + verticalOffset, 0.0f) , glm::vec4(0.1f,  0.9f, 0.12f, 1.0f), glm::vec2(tex_scale_w, 0.0f), curId });
			vec.push_back(VertexUV{ glm::vec3(start + width + horizontalOffset,  start + height + verticalOffset, 0.0f) ,  glm::vec4(0.12f, 0.9f, 0.1f,  1.0f), glm::vec2(0.0f, 0.0f), curId }); 
			vec.push_back(VertexUV{ glm::vec3(start + width + horizontalOffset, start + verticalOffset, 0.0f) , glm::vec4(0.12f, 0.1f, 0.9f,  1.0f), glm::vec2(0.0f, tex_scale_h), curId });
			curId++;
			if (curId >= numTexIds) {
				curId = 0;
			}
			horizontalOffset += width;
		}
		horizontalOffset = 0.0f;
		verticalOffset += height;
	}
}

void createSpriteSheetQuads(std::vector<VertexUV>& vec, float start, float width, float height, SpriteSheet* ss) {
	float verticalOffset = 0.0f;
	float horizontalOffset = 0.0f;
	for (int i = 0; i < ss->nSprites; i++) {
		vec.push_back(VertexUV{ 
			glm::vec3(start + horizontalOffset, start + verticalOffset, 0.0f) ,
			glm::vec4(0.11f, 0.8f, 0.76f, 1.0f),
			glm::vec2(ss->sprites[i].texCoords[0].x, ss->sprites[i].texCoords[0].y), 
			0.0f });
		vec.push_back(VertexUV{
			glm::vec3(start + horizontalOffset,  start + height + verticalOffset, 0.0f) ,
			glm::vec4(0.1f,  0.9f, 0.12f, 1.0f),
			glm::vec2(ss->sprites[i].texCoords[1].x, ss->sprites[i].texCoords[1].y),
			0.0f });
		vec.push_back(VertexUV{ 
			glm::vec3(start + width + horizontalOffset,  start + height + verticalOffset, 0.0f) ,  
			glm::vec4(0.12f, 0.9f, 0.1f,  1.0f), 
			glm::vec2(ss->sprites[i].texCoords[2].x, ss->sprites[i].texCoords[2].y),
			0.0f });
		vec.push_back(VertexUV{ 
			glm::vec3(start + width + horizontalOffset, start + verticalOffset, 0.0f) , 
			glm::vec4(0.12f, 0.1f, 0.9f,  1.0f), 
			glm::vec2(ss->sprites[i].texCoords[3].x, ss->sprites[i].texCoords[3].y),
			0.0f });
		horizontalOffset += width;
	}
}



uint32_t* generateQuadIndices(int num_quads) {
	uint32_t* verts = (uint32_t*)malloc(num_quads * 6 * sizeof(uint32_t));
	uint32_t *begin = verts;

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
	return begin;

}


//STATIC DRAW BATCH RENDERING CHALLENGE 2
void setupChallenge2(int numRows, int numCols) {
	//std::vector<VertexUV> vec = createSquareUV(0.0f, 100.0f, 100.f, 1.0f, 1.0f);
	std::vector<VertexUV> vec;
	fillVectorWithQuads(vec, 0.0f, 100.0f, 100.0f, 1.0f, 1.0f, numRows, numCols, 2);
	setupShapeUV(&Square.vao, &Square.vbo, &Square.ebo, &vec);
	uint32_t* challenge2Elements = generateQuadIndices(numRows * numCols);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((uint64_t)numRows * (uint64_t)numCols * 6 * sizeof(uint32_t)), challenge2Elements, GL_STATIC_DRAW);
	free(challenge2Elements);
} 


void drawChallenge2(int numRows, int numCols) {
	glBindVertexArray(Square.vao);
	glDrawElements(GL_TRIANGLES, numRows * numCols * 6, GL_UNSIGNED_INT, 0);
}

void destroyChallenge2() {
	destroyGlContext(&Square);
}

void setupSpriteSheetChallenge(SpriteSheet* ss) {
	std::vector<VertexUV> vec;
	createSpriteSheetQuads(vec, 50.0f, 96.0f, 96.0f, ss);
	setupShapeUV(&Square.vao, &Square.vbo, &Square.ebo, &vec);
	uint32_t* elements = generateQuadIndices(ss->nSprites);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((uint64_t)ss->nSprites * 6 * sizeof(uint32_t)), elements, GL_STATIC_DRAW);
	free(elements);
}

void drawSpriteSheetChallenge(SpriteSheet *ss) {
	glBindVertexArray(Square.vao);
	glDrawElements(GL_TRIANGLES, ss->nSprites, GL_UNSIGNED_INT, 0);
}

void destroySpriteSheetChallenge() {
	destroyGlContext(&Square);
}


struct AssetPool {
	std::unordered_map<std::string, ShaderInfo> shaders;
	std::unordered_map<std::string, Texture> textures;
	std::unordered_map<std::string, SpriteSheet> spritesheets;
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
		Texture texture = initTexture(fileName, pool->textures.size() - 1);
		it = pool->textures.find(fileName);
		return &(it->second); 
	}
}

SpriteSheet* GetSpriteSheetFromPool(AssetPool* pool, std::string fileName) {
	std::unordered_map<std::string, SpriteSheet>::iterator it = pool->spritesheets.find(fileName);
	if (pool->textures.find(fileName) != pool->textures.end()) {
		return &(it->second);
	} else {
		return nullptr;
	}
}

void addShaderAsset(AssetPool *pool, std::string vrtSrc, std::string fragSrc) {
	ShaderInfo s;
	s.id = compileShader(vrtSrc, fragSrc);
	useShader(s.id);
	s.transLoc = glGetUniformLocation(s.id, "transform");
	s.viewLoc = (glGetUniformLocation(s.id, "view"));
	s.projLoc = (glGetUniformLocation(s.id, "proj"));
	s.textureLoc = glGetUniformLocation(s.id, "uTextures");
	pool->shaders.insert({ vrtSrc, s });
}

void addSpriteSheet(AssetPool* pool, int w, int h, int numSprites, int spacing, std::string src) {
	Texture* tex = GetTextureFromPool(pool, src);
	SpriteSheet ss = SpriteSheet(tex, w, h, numSprites, spacing);
	pool->spritesheets.insert({ src, ss });
}

void deleteShaderAssets(AssetPool* pool) {
	for (auto it = pool->shaders.begin(); it != pool->shaders.end(); it++) {
		deleteShader(it->second.id);
	}
}


void addTextureAsset(AssetPool* pool, std::string fileName) {
	return;
}

static int currentChallenge = 0;

void update(double dt, entt::registry& registry, Key *keyboard) {
	auto view = registry.view<Velocity2DComponent>();
	for (auto entity : view) {
		auto& vel = view.get<Velocity2DComponent>(entity);
		bool left = keyboard->isKeyHeld(GLFW_KEY_LEFT);//bool left = Key::isKeyHeld(GLFW_KEY_LEFT);
		bool right = keyboard->isKeyHeld(GLFW_KEY_RIGHT);
		if (left) {
			vel.velocity.x = -100.0f * dt;
		} 
		if (right) {
			vel.velocity.x = 100.0f * dt;
		} 
		if (!left && !right) {
			vel.velocity.x = 0.0f;
		}

		bool up = keyboard->isKeyHeld(GLFW_KEY_UP);
		bool down = keyboard->isKeyHeld(GLFW_KEY_DOWN);
		if (down) {
			vel.velocity.y = -100.0f * dt;
		}
		if (up) {
			vel.velocity.y = 100.0f * dt;
		}
		if (!up && !down) {
			vel.velocity.y = 0.0f;
		}
		
	}
}

void checkComponent(entt::registry& registry) {
	auto view = registry.view<Velocity2DComponent>();
	for (auto entity : view) {
		auto& vel = view.get<Velocity2DComponent>(entity);
		float x = vel.velocity.x;
		float y = vel.velocity.y;
	}
}




int main() {
	/* Basic Setup */
	if (!glfwInit()) {
		printf("Failed to initialize GLFW");
		return -1;
	}
	Window* win = initializeWindow(1280, 720, "Pong");
	if (win == nullptr) {
		return -1;
	}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		glfwTerminate();
		return -1;
	}

	setCallbacks(win);


	AssetPool assets;
	addShaderAsset(&assets, "default_uv_vert.glsl", "default_uv_frag.glsl");
	
	glViewport(0, 0, win->sizeX, win->sizeY);
	//Texture tex1 = initTexture(std::string("assets/TestImage2.png"), 0);
	//Texture tex2 = initTexture(std::string("assets/wall.jpg"), 1);
	Texture characterSheet = initTexture(std::string("assets/spritesheet.png"), 0);
	SpriteSheet ss = SpriteSheet(&characterSheet, 16, 16, 26, 0);
	int samplers[2] = { 0, 1 };

	//entt practice
	entt::registry registry;
	//Entity uint32_t identifier
	entt::entity entity = registry.create();
	//Create component. Identifier + any args to be passed into the component
	registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
	registry.emplace<Velocity2DComponent>(entity, glm::vec2(0.0f, 0.0f));

	Camera* cam = initCamera(glm::vec2(0.0f, 0.0f));
	Scene* curScene = initScene(1);
	static double fps = 1.0 / 60.0;
	double beginFrameTime = glfwGetTime(), beginSecondTime = glfwGetTime();
	double endFrameTime = glfwGetTime(), endSecondTime = glfwGetTime();
	double dt = -1.0f;

	int numCols = 3;
	int numRows = 3;

	currentChallenge = 0;

	float deltaX = 0.0f;
	float deltaY = 0.0f;
	
	int frameCount = 0;
	while (!glfwWindowShouldClose(win->window)) {
		//(*curScene).update(dt);
		keyboard.keyPrevState = keyboard.keyCurState;
		//Setup Draw
		if (currentChallenge == 0) {
			setupSpriteSheetChallenge(&ss);
			currentChallenge = 2;
			/* setupChallenge2(numRows, numCols);
			currentChallenge = 2; */
		}
		updateScene(dt, &curScene);
		update(dt, registry, &keyboard);
		if (keyboard.keyIsPressed[GLFW_KEY_F]) {
			toggleFullScreen(win, true);
		}
		if (keyboard.keyIsPressed[GLFW_KEY_P]) {
			updateScene(dt, &curScene);
			printf("Changed Scene\n");
		}
		if (keyboard.keyIsPressed[GLFW_KEY_M]) {
			toggleFullScreen(win, false);
		}
		if (keyboard.keyIsPressed[GLFW_KEY_ESCAPE]) {
			glfwSetWindowShouldClose(win->window, true);
		}
		if (keyboard.isKeyDown(GLFW_KEY_T)) {
			checkComponent(registry);
		}
		glClearColor(curScene->backColor.r, curScene->backColor.g, curScene->backColor.b, 1.0f);
		//Clear Screen for next draw
		glClear(GL_COLOR_BUFFER_BIT);

		ShaderInfo* s = GetShaderFromPool(&assets, "default_uv_vert.glsl", "default_uv_frag.glsl");

		//glUniformMatrix4fv(s2.projLoc, 1, GL_FALSE, glm::value_ptr(cam->projMat));
		//glUniformMatrix4fv(s2.viewLoc, 1, GL_FALSE, glm::value_ptr(cam->viewMat));
		glUniformMatrix4fv(s->projLoc, 1, GL_FALSE, glm::value_ptr(cam->projMat));
		glUniformMatrix4fv(s->viewLoc, 1, GL_FALSE, glm::value_ptr(cam->viewMat));
		auto& trans = registry.get<TransformComponent>(entity);
		auto& vel = registry.get<Velocity2DComponent>(entity);
		trans.transform = glm::translate(trans.transform, glm::vec3(vel.velocity, 0.0f));
		glUniformMatrix4fv(s->transLoc, 1, GL_FALSE, glm::value_ptr(trans.transform));

		glUniform1iv(s->textureLoc, 2, samplers);
		deltaY = 0.0f;
		deltaX = 0.0f;
		//drawChallenge2(numRows, numCols);
		drawSpriteSheetChallenge(&ss);

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
	deleteShaderAssets(&assets);
	free(curScene);
	free(cam);

	deleteWindow(win);

	glfwTerminate();
	return 0;


}