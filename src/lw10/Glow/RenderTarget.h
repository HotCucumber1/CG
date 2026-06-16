#pragma once
#include <GL/glew.h>
#include <stdexcept>

class RenderTarget
{
public:
	RenderTarget(
		const int w,
		const int h,
		const bool withDepth = false)
		: m_width(w)
		, m_height(h)
	{
		__glewGenFramebuffers(1, &m_fbo);
		__glewBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		__glewFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

		if (withDepth)
		{
			__glewGenRenderbuffers(1, &m_rbo);
			__glewBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
			__glewRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
			__glewFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
		}

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
		if (m_rbo)
		{
			__glewDeleteRenderbuffers(1, &m_rbo);
		}
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
	GLuint m_rbo = 0;
	int m_width;
	int m_height;
};
