#pragma once

#include "dependencies.h"

#define KEY(x) GLFW_KEY_##x
#define KEY_RELEASED(x) key == KEY(x) && action == GLFW_RELEASE
#define KEY_PRESSED(x) key == KEY(x) && action == GLFW_PRESS
#define KEY_REPEAT(x) key == KEY(x) && action == GLFW_REPEAT
#define KEY_DOWN(x) key == KEY(x) && (action == GLFW_PRESS || action == GLFW_REPEAT)

#define KEY_CALLBACK(...) \
	Window::SetKeyCallback([](GLFWwindow* window, int key, int scancode, int action, int mode) \
	{ \
		__VA_ARGS__; \
	});

struct WindowData
{
	int width, height;
	std::string name;
};

class Window
{
private:
	static GLFWwindow* s_Window;
	static WindowData s_WindowData;

	static GLFWwindowsizefun s_CustomSizeCallback;

public:
	static bool Init(int width = 1280, int height = 720, const std::string& name = "Application");

	static void SetSizeCallback(GLFWwindowsizefun function);
	static void SetKeyCallback(GLFWkeyfun function);

	static inline unsigned int GetWidth() { return s_WindowData.width; }
	static inline unsigned int GetHeight() { return s_WindowData.height; }
	static inline bool ShouldClose() { return glfwWindowShouldClose(s_Window); }
	static inline void PollEvents() { glfwPollEvents(); }
	static inline void SwapBuffers() { glfwSwapBuffers(s_Window); }

private:
	static void SizeCallback(GLFWwindow* window, int width, int height);
};