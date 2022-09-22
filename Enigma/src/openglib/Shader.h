#pragma once

#include <string>
#include <unordered_map>
#include "dependencies.h"

class Shader
{
private:
	unsigned int m_RendererId;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	void CreateFromPath(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void CreateFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

	~Shader();

	const void Bind();

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, unsigned int count, const int* value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4f(const std::string& name, const glm::mat4& mat);

private:
	unsigned int CompileShader(const int type, const std::string& source);
	unsigned int CreateProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	std::string GetShaderSource(const std::string& shaderPath);

	unsigned int GetUniformLocation(const std::string& name);
};