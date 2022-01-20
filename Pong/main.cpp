#include "Window.h"
#include "Shader.h"
#include "Graphics.h"
#include "Camera.h"
#include "Texture.h"
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

struct Sprite {
	Texture* texture;
	glm::vec2 texCoords[4];
	Sprite() = default;
	Sprite(Texture* _texture) {
		texture = _texture;
		texCoords[0] = { 1.0f, 1.0f };
		texCoords[1] = { 1.0f, 0.0f };
		texCoords[2] = { 0.0f, 0.0f };
		texCoords[3] = { 0.0f, 1.0f };
	}

	Sprite(Texture* _texture, glm::vec2 _coords) {
		texture = _texture;
		texCoords[0] = { _coords.x, _coords.y };
		texCoords[1] = { _coords.x, 0.0f };
		texCoords[2] = { 0.0f, 0.0f };
		texCoords[3] = { 0.0f, _coords.y };
	}
	Sprite(Texture* _texture, glm::vec2 *texCoords) {
		texture = _texture;
		for (int i = 0; i < 4; i++) {
			texCoords[0] = (*texCoords);
			texCoords++;
		}
	}
};

struct SpriteSheet {
	Texture* texture;
	Sprite* sprites;
	SpriteSheet() = default;
	SpriteSheet(Texture* tex, int spr_w, int spr_h, int numSprites, int spacing) {
		sprites = (Sprite*)malloc(numSprites * sizeof(Sprite));
		int currentX = 0;
		int currentY = texture->height - spr_h;
		for (int i = 0; i < numSprites; i++) {
			//Get the normalized  coordinates of the sprite within the texture
			float nLeft = currentX / float(texture->width);
			float nRight = (currentX + spr_w) / float(texture->width);
			float nBottom = currentY / float(texture->height);
			float nTop = (currentY + spr_h) / float(texture->height);
			glm::vec2 coords[4] = {
				{nRight, nTop},
				{nRight, nBottom},
				{nLeft, nBottom},
				{nLeft, nTop}
			};
			sprites = new Sprite(texture, coords);
			sprites++;
			currentX += spr_w + spacing;
			if (currentX >= texture->width) {
				currentX = 0.0f;
				currentY -= spr_h + spacing;
			}
		}
	}
	~SpriteSheet() {
		free(sprites);
	}

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

namespace Input {
	namespace Key {
		bool keyIsPressed[GLFW_KEY_LAST];
		bool keyIsHeld[GLFW_KEY_LAST];
		std::array<bool, GLFW_KEY_LAST> keyCurState = { 0 };
		std::array<bool, GLFW_KEY_LAST> keyPrevState = { 0 };
		bool isKeyDown(int key) {
			if (key >= 0 && key < GLFW_KEY_LAST) {
				bool k = keyIsPressed[key];
				keyIsPressed[key] = false;
				return k;
			}
		}
		bool isKeyHeld(int key) {
			if (key >= 0 && key < GLFW_KEY_LAST) {
				return (keyCurState[key] == keyPrevState[key] && keyCurState[key] == GLFW_PRESS);
			}
		}
		
		bool singlePress(int key) {
			if (key >= 0 && key < GLFW_KEY_LAST) {
				return (keyIsPressed[key] && !keyIsHeld[key]);
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
		//Key::keyIsHeld[key] = (Key::keyIsPressed[key] && action == GLFW_PRESS);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numRows * numCols * 6 * sizeof(uint32_t)), challenge2Elements, GL_STATIC_DRAW);
	free(challenge2Elements);
} 


void drawChallenge2(int numRows, int numCols) {
	glBindVertexArray(Square.vao);
	glDrawElements(GL_TRIANGLES, numRows * numCols * 6, GL_UNSIGNED_INT, 0);
}

void destroyChallenge2() {
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

void update(double dt, entt::registry& registry) {
	auto view = registry.view<Velocity2DComponent>();
	for (auto entity : view) {
		auto& vel = view.get<Velocity2DComponent>(entity);
		bool left = Key::isKeyHeld(GLFW_KEY_LEFT);
		bool right = Key::isKeyHeld(GLFW_KEY_RIGHT);
		if (left) {
			vel.velocity.x = -100.0f * dt;
		} 
		if (right) {
			vel.velocity.x = 100.0f * dt;
		} 
		if (!left && !right) {
			vel.velocity.x = 0.0f;
		}

		bool up = Key::isKeyHeld(GLFW_KEY_UP);
		bool down = Key::isKeyHeld(GLFW_KEY_DOWN);
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

	AssetPool assets;
	addShaderAsset(&assets, "default_uv_vert.glsl", "default_uv_frag.glsl");
	
	glViewport(0, 0, win->sizeX, win->sizeY);
	Texture tex1 = initTexture(std::string("assets/TestImage2.png"), 0);
	Texture tex2 = initTexture(std::string("assets/wall.jpg"), 1);
	int samplers[2] = { 0, 1 };

	//entt practice
	entt::registry registry;
	//Entity uint32_t identifier
	entt::entity entity = registry.create();
	//Create component. Identifier + any args to be passed into the component
	registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
	registry.emplace<Velocity2DComponent>(entity, glm::vec2(0.0f, 0.0f));

	Camera* cam = initCamera(glm::vec2(0.0f, 0.0f));
	Scene* curScene = initScene(250.0f / 255.0f, 119.0f/255.0f, 110.0f/255.0f);
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
		Key::keyPrevState = Key::keyCurState;
		//Setup Draw
		if (currentChallenge == 0) {
			setupChallenge2(numRows, numCols);
			currentChallenge = 2;
		}
		updateScene(dt, &curScene);
		update(dt, registry);
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
		if (Key::isKeyDown(GLFW_KEY_T)) {
			checkComponent(registry);
		}
		if (Key::isKeyDown(GLFW_KEY_R)) {
			destroyChallenge2();
			numRows = numRows + 1;
			setupChallenge2(numRows, numCols);
		}
		if (Key::isKeyDown(GLFW_KEY_C)) {
			destroyChallenge2();
			numCols = numCols + 1;
			setupChallenge2(numRows, numCols);
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
		drawChallenge2(numRows, numCols);

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
	//deleteShader(s2.id);
	unbindTexture(&tex1);
	//unbindTexture(tex2);
	//free(&tex1);
	free(curScene);
	free(cam);

	deleteWindow(win);

	glfwTerminate();
	return 0;


}