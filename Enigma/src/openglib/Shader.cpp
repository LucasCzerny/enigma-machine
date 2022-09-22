#include "Shader.h"

#include <fstream>
#include <csignal>

void Shader::CreateFromPath(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	const std::string vertexShaderSource = GetShaderSource(vertexShaderPath);
	const std::string fragmentShaderSource = GetShaderSource(fragmentShaderPath);
	m_RendererId = CreateProgram(vertexShaderSource, fragmentShaderSource);
}

void Shader::CreateFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	m_RendererId = CreateProgram(vertexShaderSource, fragmentShaderSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererId);
}

const void Shader::Bind()
{
	glUseProgram(m_RendererId);
}

unsigned int Shader::CompileShader(const int type, const std::string& source)
{
	const char* shaderSource = source.c_str();

	unsigned int shader = glCreateShader(type);
	GLCall(glShaderSource(shader, 1, &shaderSource, NULL));
	GLCall(glCompileShader(shader));

	// error handling
	int result;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));

		char* message = new char[length];
		GLCall(glGetShaderInfoLog(shader, length, &length, message));

		std::cout << "Failed to compile shader of type \"" << (type == GL_VERTEX_SHADER ? "VertexShader" : "FragmentShader") << "\": " << message << "\n";
		return 0;
	}

	return shader;
}

unsigned int Shader::CreateProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	GLCall(unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource));
	GLCall(unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource));

	GLCall(unsigned int program = glCreateProgram());
	GLCall(glAttachShader(program, vertexShader));
	GLCall(glAttachShader(program, fragmentShader));

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	return program;
}

std::string Shader::GetShaderSource(const std::string& shaderPath)
{
	std::ifstream sourceFile(shaderPath);
	std::string source;

	if (sourceFile.is_open())
	{
		std::string line;
		while (getline(sourceFile, line))
		{
			source += line + "\n";
		}
	}

	sourceFile.close();
	return source;
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string& name, unsigned int count, const int* value)
{
	GLCall(glUniform1iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4& mat)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	unsigned int location = glGetUniformLocation(m_RendererId, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning: uniform \"" << name << "\" doesn't exist!\n";
	}
	return location;
}