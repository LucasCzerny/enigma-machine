#pragma once

#include <string>

class Texture
{	
private:
	unsigned int m_RendererId;
	std::string m_FilePath;
	unsigned char* m_TextureBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	inline unsigned int GetRendererID() const { return m_RendererId; }

	bool operator==(const Texture& other)
	{
		return m_RendererId == other.GetRendererID();
	}
};

