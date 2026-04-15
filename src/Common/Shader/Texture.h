#pragma once
#include <GL/gl.h>
#include <string>
#include <stb_image.h>

class Texture
{
public:
	Texture() = default;

	explicit Texture(const std::string& path)
		: Texture()
	{
		if (!LoadFromFile(path))
		{
			throw std::runtime_error("Failed to load texture: " + path);
		}
	}

	~Texture()
	{
		Cleanup();
	}

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept
		: m_textureId(std::exchange(other.m_textureId, 0))
		, m_width(std::exchange(other.m_width, 0))
		, m_height(std::exchange(other.m_height, 0))
		, m_channels(std::exchange(other.m_channels, 0))
	{
	}

	Texture& operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			Cleanup();
			m_textureId = std::exchange(other.m_textureId, 0);
			m_width = std::exchange(other.m_width, 0);
			m_height = std::exchange(other.m_height, 0);
			m_channels = std::exchange(other.m_channels, 0);
		}
		return *this;
	}

	bool LoadFromFile(const std::string& path)
	{
		Cleanup();

		stbi_set_flip_vertically_on_load(true);
		const auto data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 4);
		if (!data)
		{
			return false;
		}

		glGenTextures(1, &m_textureId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);

		// TODO это че такое
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexImage2D(
			GL_TEXTURE_2D, 0,
			GL_RGBA, m_width, m_height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		__glewGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}

	void Bind(const int unit = 0) const
	{
		assert(m_textureId != 0);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
	}

	static void Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	int GetWidth() const
	{
		return m_width;
	}

	int GetHeight() const
	{
		return m_height;
	}

	GLuint GetId() const
	{
		return m_textureId;
	}

	bool IsValid() const
	{
		return m_textureId != 0;
	}

private:
	void Cleanup()
	{
		if (m_textureId != 0)
		{
			glDeleteTextures(1, &m_textureId);
			m_textureId = 0;
		}
		m_width = 0;
		m_height = 0;
		m_channels = 0;
	}

private:
	GLuint m_textureId = 0;
	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;
};