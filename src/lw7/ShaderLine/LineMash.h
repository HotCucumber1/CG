#pragma once
#include <GL/glew.h>

class LineMash
{
public:
	LineMash() = default;

	~LineMash();

	void Init();

	void Draw() const;

	void CleanUp() const;

private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLsizei m_vertexCount = 0;
};
