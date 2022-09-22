#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererId);
	glBindVertexArray(m_RendererId);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererId);
}

const void VertexArray::Bind()
{
	glBindVertexArray(m_RendererId);
}

const void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(VertexBuffer& vertexBuffer, VertexBufferLayout& vertexBufferLayout)
{
	Bind();
	vertexBuffer.Bind();

	const std::vector<int> elements = vertexBufferLayout.GetElements();
	unsigned int stride = vertexBufferLayout.GetStride();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		int size = elements[i];
		glVertexAttribPointer(i, size, GL_FLOAT, false, stride, (const void*)offset);
		glEnableVertexAttribArray(i);
		offset += size * sizeof(float);
	}
}