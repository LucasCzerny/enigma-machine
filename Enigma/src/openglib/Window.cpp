#include "Window.h"

GLFWwindow* Window::s_Window;
WindowData Window::s_WindowData;

GLFWwindowsizefun Window::s_CustomSizeCallback;

bool Window::Init(int width, int height, const std::string& name)
{
	s_WindowData = { width, height, name };

	if (!glfwInit())
	{
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	s_Window = glfwCreateWindow(s_WindowData.width, s_WindowData.height, s_WindowData.name.c_str(), NULL, NULL);

	if (!s_Window)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(s_Window);

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	glfwSetWindowSizeCallback(s_Window, Window::SizeCallback);
}

void Window::SetSizeCallback(GLFWwindowsizefun function)
{
	s_CustomSizeCallback = function;
}

void Window::SizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	s_WindowData.width = width;
	s_WindowData.height = height;

	if (s_CustomSizeCallback)
	{
		s_CustomSizeCallback(window, width, height);
	}
}

void Window::SetKeyCallback(GLFWkeyfun function)
{
	glfwSetKeyCallback(s_Window, function);
}