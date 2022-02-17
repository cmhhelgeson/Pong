#include "core.h"

namespace Input {
	struct Key {
		bool keyIsPressed[GLFW_KEY_LAST];
		bool keyIsHeld[GLFW_KEY_LAST];
		std::array<bool, GLFW_KEY_LAST> keyCurState = { 0 };
		std::array<bool, GLFW_KEY_LAST> keyPrevState = { 0 };
		bool isKeyDown(int key);
		bool isKeyHeld(int key);
		bool singlePress(int key);
	};

	struct Mouse {
		bool mouseIsPressed[GLFW_MOUSE_BUTTON_LAST];
	};
};

