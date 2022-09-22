#include "VertexBufferLayout.h"

void VertexBufferLayout::Push(int size)
{
	m_SizeVector.push_back(size);
	m_Stride += size * sizeof(float);
}