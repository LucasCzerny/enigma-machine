#include "Renderer.h"

Color::Color(float r, float g, float b, float alpha)
	: m_Red(r), m_Green(g), m_Blue(b), m_Alpha(alpha) {}

Color::Color(unsigned int r, unsigned int g, unsigned int b, float alpha)
	: m_Alpha(alpha)
{
	m_Red = r / 255.0f;
	m_Green = g / 255.0f;
	m_Blue = b / 255.0f;
}

Color::Color(const std::string& hex)
	: m_Alpha(1.0f)
{
	glm::vec3 color = HexToRGB(hex);

	m_Red = color.r / 255.0f;
	m_Green = color.g / 255.0f;
	m_Blue = color.b / 255.0f;
}

glm::vec3 Color::HexToRGB(const std::string& hex)
{
	ASSERT(hex.length() == 7 && hex[0] == '#')

	glm::vec3 result;

	for (int i = 0; i < 3; i++)
	{
		result[i] = strtol(hex.substr(2 * i + 1, 2).c_str(), NULL, 16);
	}

	return result;
}