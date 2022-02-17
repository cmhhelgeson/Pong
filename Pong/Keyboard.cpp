#include "Keyboard.h"

namespace Input {
	bool Key::isKeyDown(int key) {
		if (key >= 0 && key < GLFW_KEY_LAST) {
			bool k = keyIsPressed[key];
			keyIsPressed[key] = false;
			return k;
		}
	}
	bool Key::isKeyHeld(int key) {
		if (key >= 0 && key < GLFW_KEY_LAST) {
			return (keyCurState[key] == keyPrevState[key] && keyCurState[key] == GLFW_PRESS);
		}
	}
	bool Key::singlePress(int key) {
		if (key >= 0 && key < GLFW_KEY_LAST) {
			return (keyIsPressed[key] && !keyIsHeld[key]);
		}
	}
}