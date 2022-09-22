#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <csignal>
#include <vector>
#include <array>
#include <functional>
#include <algorithm>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define ASSERT(x) if (!(x)) std::raise(SIGINT);
#define GLCall(x) \
	GLClearError(); \
	x; \
	ASSERT(GlLogCall(#x, __FILE__, __LINE__));

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GlLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error][" << file << ":" << line << ": " << function << "]: " << error << "\n";
		return false;
	}

	return true;
}