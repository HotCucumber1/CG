#pragma once
#include <cassert>
#include <GL/glew.h>
#include <stdexcept>
#include <utility>

class Shader
{
public:
	explicit Shader(const GLenum type)
		: m_shader{ __glewCreateShader(type) }
	{
		if (!m_shader)
		{
			throw std::runtime_error("Failed to create shader");
		}
	}

	Shader(Shader&& other) noexcept
		: m_shader(std::exchange(other.m_shader, 0))
	{
	}

	~Shader()
	{
		__glewDeleteShader(m_shader);
	}

	Shader& operator=(Shader&& rhs) noexcept
	{
		std::swap(m_shader, rhs.m_shader);
		return *this;
	}

	void SetSource(const char* text) const noexcept
	{
		assert(text);
		assert(m_shader);
		__glewShaderSource(m_shader, 1, &text, nullptr);
	}

	void Compile() const noexcept
	{
		assert(m_shader);
		__glewCompileShader(m_shader);
	}

	void GetParameter(const GLenum paramName, GLint* p) const noexcept
	{
		__glewGetShaderiv(m_shader, paramName, p);
	}

	std::string GetInfoLog() const
	{
		int infoLogLength = 0;
		GetParameter(GL_INFO_LOG_LENGTH, &infoLogLength);

		std::string log(static_cast<size_t>(infoLogLength), ' ');
		GLsizei actualSize = 0;

		__glewGetShaderInfoLog(m_shader, infoLogLength, &actualSize, log.data());
		log.resize(static_cast<size_t>(actualSize));
		return log;
	}

	bool IsCompiled() const noexcept
	{
		int compileStatus = GL_FALSE;
		GetParameter(GL_COMPILE_STATUS, &compileStatus);
		return compileStatus == GL_TRUE;
	}

	operator GLuint() const noexcept
	{
		return m_shader;
	}

private:
	GLuint m_shader = 0;
};