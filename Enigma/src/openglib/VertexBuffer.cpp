#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const float* vertices, unsigned int size, bool dynamic /*= false*/)
{
	glGenBuffers(1, &m_RendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererId);
}

const void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
}

const void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::UpdateVertices(const float* vertices, unsigned int size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), vertices);
}