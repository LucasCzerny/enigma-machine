#pragma once

#include <vector>

#include "dependencies.h"

class VertexBufferLayout
{
private:
	std::vector<int> m_SizeVector;
	unsigned int m_Stride = 0;

public:
	void Push(int size);

	inline const unsigned int GetStride() { return m_Stride; }
	inline const std::vector<int> GetElements() { return m_SizeVector; }
};