#ifndef WINDOW_H
#define WINDOW_H

#include "core.h"

struct Window {
	GLFWwindow* window;
	GLFWmonitor* monitor;
	int posX;
	int posY;
	int sizeX;
	int sizeY;
	int viewportX;
	int viewportY;
};

Window* initializeWindow(int w, int h, const char* title);
bool isFullScreen(GLFWwindow* window);
void toggleFullScreen(Window* win, bool fullscreen);
void deleteWindow(Window* window);

#endif
