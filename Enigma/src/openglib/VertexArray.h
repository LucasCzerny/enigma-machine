#pragma once

#include "dependencies.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererId;

public:
	VertexArray();
	~VertexArray();

	const void Bind();
	const void Unbind();

	void AddBuffer(VertexBuffer& vertexBuffer, VertexBufferLayout& vertexBufferLayout);
};

