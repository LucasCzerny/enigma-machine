#pragma once

#include "dependencies.h"

class VertexBuffer
{
private:
	unsigned int m_RendererId;

public:
	VertexBuffer(const float* vertices, unsigned int size, bool dynamic = false);
	~VertexBuffer();

	const void Bind();
	const void Unbind();

	void UpdateVertices(const float* vertices, unsigned int size);
};