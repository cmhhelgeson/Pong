#include "Window.h"


Window* initializeWindow(int w, int h, const char* title) {
	//OpenGL Version 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//OpenGL Version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//No deprecated functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window* win = new Window();

	win->window = glfwCreateWindow(
		w,
		h,
		title,
		nullptr,
		nullptr
	);
	if (win->window == nullptr) {
		printf("Window creation failed\n");
		glfwTerminate();
		return nullptr;
	}


	glfwMakeContextCurrent(win->window);
	win->monitor = glfwGetPrimaryMonitor();
	glfwGetWindowSize(win->window, &(win->sizeX), &(win->sizeY));
	glfwGetWindowPos(win->window, &(win->posX), &(win->posY));

	return win;

}

bool isFullScreen(GLFWwindow* window) {
	return glfwGetWindowMonitor(window) != nullptr;
}

void toggleFullScreen(Window* win, bool fullscreen) {
	if (isFullScreen(win->window) == fullscreen) {
		return;
	}
	if (fullscreen) {
		//Save last pos and size in windowed mode
		glfwGetWindowPos(win->window, &(win->posX), &(win->posY));
		glfwGetWindowSize(win->window, &(win->sizeX), &(win->sizeY));
		//Get screen Resolution
		const GLFWvidmode* mode = glfwGetVideoMode(win->monitor);
		glfwSetWindowMonitor(win->window, win->monitor, 0, 0, mode->width, mode->height, 0);
		glViewport(0, 0, mode->width, mode->height);
	}
	else {
		glfwSetWindowMonitor(win->window, nullptr, win->posX, win->posY, win->sizeX, win->sizeY, 0);
		glViewport(0, 0, win->sizeX, win->sizeY);
	}


}

void deleteWindow(Window* window) {
	glfwDestroyWindow(window->window);
	delete window;
}