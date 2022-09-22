#pragma once

#include "dependencies.h"

class IndexBuffer
{
private:
	unsigned int m_RendererId;
public:
	IndexBuffer(unsigned int* indices, int size);
	~IndexBuffer();

	const void Bind();
};

