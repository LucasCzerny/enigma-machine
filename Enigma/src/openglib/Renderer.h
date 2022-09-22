#pragma once

#include "dependencies.h"
#include "openglib.h"

#define MAX_VERTEX_COUNT 10000
#define MAX_INDEX_COUNT MAX_VERTEX_COUNT * 6

typedef unsigned int Quad;

struct Vertex
{
	Vertex() {}
	Vertex(glm::vec2 position, glm::vec4 color, glm::vec2 texCoords, float texIndex)
		: position(position), color(color), texCoords(texCoords), texIndex(texIndex) {}

	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	float texIndex;
};

struct Color
{
private:
	float m_Red, m_Green, m_Blue, m_Alpha;

public:
	Color(float r, float g, float b, float alpha);
	Color(unsigned int r, unsigned int g, unsigned int b, float alpha = 1.0f);
	Color(const std::string& hex);

	inline glm::vec4 GetColor() const { return { m_Red, m_Green, m_Blue, m_Alpha }; }

private:
	static glm::vec3 HexToRGB(const std::string& hex);
};

class Renderer
{
private:
	static Vertex* s_Vertices;
	static Vertex* s_VerticesPointer;

	static unsigned int* s_Indices;
	static unsigned int* s_IndicesPointer;
	
	static VertexBuffer* s_VertexBuffer;
	static IndexBuffer* s_IndexBuffer;
	static VertexBufferLayout* s_VertexBufferLayout;
	static VertexArray* s_VertexArray;

	static Shader* s_Shader;
	static std::string s_VertexShaderSource;
	static std::string s_FragmentShaderSource;

	static std::vector<Texture> s_Textures;
	static unsigned char s_WhiteTextureData[4];

	static unsigned int s_CurrentQuadID;

public:
	static void Init(int width = 1280, int height = 720, const std::string& name = "Application");

	static unsigned int DrawQuad(float x, float y, float width, float height, const Color& color);
	static unsigned int DrawQuad(float x, float y, float width, float height, const Texture& texture);

	static void MoveQuad(unsigned int id, float x, float y);
	static void MoveQuadAbsolute(unsigned int id, float x, float y);
	static void ScaleQuad(unsigned int id, float x, float y);
	static void ChangeColor(unsigned int id, Color color);
	static void ChangeTexture(unsigned int id, Texture texture);

	static bool ShouldDraw();
	static void Draw();

private:
	static void SizeCallback(GLFWwindow*, int width, int height);
};
