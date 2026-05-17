#pragma once
#include <GL/glew.h>

class AbstractMesh
{
public:
	AbstractMesh() = default;

	virtual ~AbstractMesh()
	{
		CleanUp();
	}

	virtual void Init() = 0;

	virtual void Draw() const = 0;

private:
	void CleanUp() const
	{
		if (m_vbo)
		{
			glDeleteBuffers(1, &m_vbo);
		}
		if (m_vao)
		{
			glDeleteVertexArrays(1, &m_vao);
		}
	}

protected:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLsizei m_vertexCount = 0;
};