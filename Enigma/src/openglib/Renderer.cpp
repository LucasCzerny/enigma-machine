#include "Renderer.h"

#define VertexMultiplierX(index) (int)(index == 1 || index == 2)
#define VertexMultiplierY(index) (int)(index == 2 || index == 3)

VertexBuffer* Renderer::s_VertexBuffer = nullptr;
IndexBuffer* Renderer::s_IndexBuffer = nullptr;
VertexBufferLayout* Renderer::s_VertexBufferLayout = nullptr;
VertexArray* Renderer::s_VertexArray = nullptr;

unsigned int Renderer::s_CurrentQuadID = 0;

std::vector<Texture> Renderer::s_Textures;

Vertex* Renderer::s_Vertices = nullptr;
Vertex* Renderer::s_VerticesPointer = nullptr;

unsigned int* Renderer::s_Indices = nullptr;
unsigned int* Renderer::s_IndicesPointer = nullptr;

Shader* Renderer::s_Shader = nullptr;

unsigned char Renderer::s_WhiteTextureData[4] = { 255, 255, 255, 255 };

void Renderer::Init(int width, int height, const std::string& name)
{
	Window::Init(width, height, name);

	s_Vertices = new Vertex[MAX_VERTEX_COUNT];
	s_VerticesPointer = s_Vertices;

	s_Indices = new unsigned int[MAX_INDEX_COUNT];
	s_IndicesPointer = s_Indices;

	unsigned int offset = 0;

	for (int i = 0; i < MAX_INDEX_COUNT; i += 6)
	{
		s_Indices[i + 0] = 0 + offset;
		s_Indices[i + 1] = 1 + offset;
		s_Indices[i + 2] = 2 + offset;

		s_Indices[i + 3] = 2 + offset;
		s_Indices[i + 4] = 3 + offset;
		s_Indices[i + 5] = 0 + offset;

		offset += 4;
	}

	s_VertexBuffer = new VertexBuffer(nullptr, MAX_VERTEX_COUNT * sizeof(Vertex), true);

	s_VertexBufferLayout = new VertexBufferLayout();
	s_VertexBufferLayout->Push(2);
	s_VertexBufferLayout->Push(4);
	s_VertexBufferLayout->Push(2);
	s_VertexBufferLayout->Push(1);

	s_VertexArray = new VertexArray();
	s_VertexArray->AddBuffer(*s_VertexBuffer, *s_VertexBufferLayout);

	s_IndexBuffer = new IndexBuffer(s_Indices, MAX_INDEX_COUNT);
		
	s_Shader = new Shader();
	s_Shader->CreateFromSource(s_VertexShaderSource, s_FragmentShaderSource);

	s_Shader->Bind();
	glm::mat4 proj = glm::ortho(0.0f, (float)Window::GetWidth(), 0.0f, (float)Window::GetHeight(), -1.0f, 1.0f);
	s_Shader->SetUniformMatrix4f("u_Proj", proj);

	int sampler[32];
	for (int i = 0; i < 32; i++)
	{
		sampler[i] = i;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, s_WhiteTextureData);
	glBindTexture(GL_TEXTURE_2D, 0);

	s_Shader->SetUniform1iv("u_Textures", 32, sampler);

	Window::SetSizeCallback(Renderer::SizeCallback);
}

unsigned int Renderer::DrawQuad(float x, float y, float width, float height, const Color& color)
{
	unsigned int id = s_CurrentQuadID;

	for (unsigned int i = 0; i < 4; i++)
	{
		s_VerticesPointer->position = { x + width * VertexMultiplierX(i), y + height * VertexMultiplierY(i) };
		s_VerticesPointer->color = color.GetColor();
		s_VerticesPointer->texCoords = { 0.0f, 0.0f };
		s_VerticesPointer->texIndex = 0.0f;

		s_VerticesPointer++;
	}

	s_CurrentQuadID++;

	return id;
}

unsigned int Renderer::DrawQuad(float x, float y, float width, float height, const Texture& texture)
{
	unsigned int id = s_CurrentQuadID;

	s_Textures.push_back(texture);
	int textureIndex = s_Textures.size();

	for (unsigned int i = 0; i < 4; i++)
	{
		s_VerticesPointer->position = { x + width * VertexMultiplierX(i), y + height * VertexMultiplierY(i) };
		s_VerticesPointer->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_VerticesPointer->texCoords = { VertexMultiplierX(i), VertexMultiplierY(i) };
		s_VerticesPointer->texIndex = textureIndex;

		s_VerticesPointer++;
	}

	s_CurrentQuadID++;

	return id;
}

void Renderer::MoveQuad(unsigned int id, float x, float y)
{
	int offset = id * 4;
	Vertex* rectanglePointer = s_Vertices + offset;

	for (unsigned int i = 0; i < 4; i++)
	{
		glm::vec2 oldPosition = rectanglePointer->position;
		rectanglePointer->position = { oldPosition[0] + x, oldPosition[1] + y };

		rectanglePointer++;
	}
}

void Renderer::MoveQuadAbsolute(unsigned int id, float x, float y)
{
	int offset = id * 4;
	Vertex* rectanglePointer = s_Vertices + offset;

	int differenceX = x - rectanglePointer->position.x,
		differenceY = y - rectanglePointer->position.y;

	for (unsigned int i = 0; i < 4; i++)
	{
		glm::vec2 oldPosition = rectanglePointer->position;
		rectanglePointer->position = { oldPosition[0] + differenceX, oldPosition[1] + differenceY };

		rectanglePointer++;
	}
}

void Renderer::ScaleQuad(unsigned int id, float x, float y)
{
	int offset = id * 4;
	Vertex* rectanglePointer = s_Vertices + offset;

	glm::vec2 basePosition = rectanglePointer->position;

	for (unsigned int i = 0; i < 4; i++)
	{
		glm::vec2 oldPosition = rectanglePointer->position;
		int width = oldPosition[0] - basePosition[0];
		int height = oldPosition[1] - basePosition[1];

		rectanglePointer->position = { basePosition[0] + x * width, basePosition[1] + y * height };

		rectanglePointer++;
	}
}

void Renderer::ChangeColor(unsigned int id, Color color)
{
	int offset = id * 4;
	Vertex* rectanglePointer = s_Vertices + offset;

	glm::vec4 newColor = color.GetColor();

	for (unsigned int i = 0; i < 4; i++)
	{
		rectanglePointer->color = newColor;

		rectanglePointer++;
	}
}

void Renderer::ChangeTexture(unsigned int id, Texture texture)
{
	int offset = id * 4;
	Vertex* rectanglePointer = s_Vertices + offset;

	auto it = std::find(s_Textures.begin(), s_Textures.end(), texture);
	int index;

	if (it != s_Textures.end())
	{
		int index = it - s_Textures.begin();
		s_Textures[index] = texture;
	}
	else
	{
		s_Textures.push_back(texture);
		index = s_Textures.size() - 1;
	}

	for (unsigned int i = 0; i < 4; i++)
	{
		rectanglePointer->texIndex = (float)index + 1;

		rectanglePointer++;
	}
}

bool Renderer::ShouldDraw()
{
	return !Window::ShouldClose();
}

void Renderer::Draw()
{
	unsigned int size = s_VerticesPointer - s_Vertices;

	s_VertexBuffer->UpdateVertices((float*)s_Vertices, size * sizeof(Vertex) / 4);

	s_VertexArray->Bind();
	s_IndexBuffer->Bind();
	s_Shader->Bind();

	for (int i = 0; i < s_Textures.size(); i++)
	{
		s_Textures[i].Bind(i + 1);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, size * 6 / 4, GL_UNSIGNED_INT, nullptr);

	Window::PollEvents();
	Window::SwapBuffers();
}

void Renderer::SizeCallback(GLFWwindow*, int width, int height)
{
	glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
	s_Shader->SetUniformMatrix4f("u_Proj", proj);
}


std::string Renderer::s_VertexShaderSource =
	"#version 450 core\n"

	"layout(location = 0) in vec2 a_Position;\n"
	"layout(location = 1) in vec4 a_Color;\n"
	"layout(location = 2) in vec2 a_TexCoords;\n"
	"layout(location = 3) in float a_TexIndex;\n"

	"out vec4 v_Color\n;"
	"out vec2 v_TexCoords;\n"
	"out float v_TexIndex;\n"

	"uniform mat4 u_Proj;\n"

	"void main()\n"
	"{\n"
		"v_Color = a_Color;\n"
		"v_TexCoords = a_TexCoords;\n"
		"v_TexIndex = a_TexIndex;\n"

		"gl_Position = u_Proj * vec4(a_Position, 0.0, 1.0);\n"
	"}\n";

std::string Renderer::s_FragmentShaderSource =
	"#version 450 core\n"

	"layout(location = 0) out vec4 o_Color;\n"

	"in vec4 v_Color;\n"
	"in vec2 v_TexCoords;\n"
	"in float v_TexIndex;\n"

	"uniform sampler2D u_Textures[32];\n"

	"void main()\n"
	"{\n"
		"int index = int(v_TexIndex);\n"
		"o_Color = texture(u_Textures[index], v_TexCoords) * v_Color;\n"
	"}\n";