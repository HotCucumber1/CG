#pragma once
#include <GL/glew.h>
#include <stdexcept>

class RenderTarget
{
public:
	RenderTarget(const int w, const int h)
		: m_width(w)
		, m_height(h)
	{
		constexpr float borderColor[] = { 0, 0, 0, 1 };

		__glewGenFramebuffers(1, &m_fbo);
		__glewBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		__glewFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

		if (__glewCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::runtime_error("Framebuffer not complete!");
		}
		__glewBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	~RenderTarget()
	{
		__glewDeleteFramebuffers(1, &m_fbo);
		glDeleteTextures(1, &m_texture);
	}

	void Bind() const
	{
		__glewBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glViewport(0, 0, m_width, m_height);
	}

	GLuint GetTexture() const
	{
		return m_texture;
	}

private:
	GLuint m_fbo = 0;
	GLuint m_texture = 0;
	int m_width;
	int m_height;
};
