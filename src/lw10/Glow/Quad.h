#pragma once
#include <GL/glew.h>

class Quad
{
public:
	Quad()
	{
		constexpr float quadVertices[] = {
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};

		__glewGenVertexArrays(1, &m_vao);
		__glewGenBuffers(1, &m_vbo);
		__glewBindVertexArray(m_vao);
		__glewBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		__glewBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		__glewEnableVertexAttribArray(0);
		__glewVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		__glewEnableVertexAttribArray(1);
		__glewVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	}

	void Draw() const
	{
		__glewBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		__glewBindVertexArray(0);
	}

private:
	GLuint m_vao{};
	GLuint m_vbo{};
};
