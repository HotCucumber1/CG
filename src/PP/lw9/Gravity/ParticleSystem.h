#pragma once
#include <CL/cl2.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Particle
{
	glm::vec4 position;
	glm::vec4 velocity;
};

class ParticleSystem
{
public:
	ParticleSystem(int numParticles, float G, float timeScale);

	~ParticleSystem();

	void InitializeRandom(float radius, float maxMass, float velScale);

	void Update(float deltaTime);

	void Render() const;

	int GetParticleCount() const
	{
		return m_numParticles;
	}

	void SetGravitationalConstant(const float G)
	{
		m_G = G;
	}

	void SetTimeScale(const float scale)
	{
		m_timeScale = scale;
	}

	GLuint GetTextureID() const
	{
		return m_textureID;
	}
	const std::vector<Particle>& GetParticles() const
	{
		return m_particles;
	}

private:
	static constexpr int MAX_PARTICLES = 100000;
	void SetupOpenCL();
	void SetupOpenGLBuffers();
	void CreateTexture();

	std::vector<Particle> m_particles;
	int m_numParticles;
	float m_G;
	float m_timeScale;

	cl::Context m_context;
	cl::CommandQueue m_queue;
	cl::Program m_program;
	cl::Kernel m_kernel;
	cl::Buffer m_particleBufferCLIn;
	cl::Buffer m_particleBufferCLOut;

	GLuint m_particleBufferGL{};
	GLuint m_textureID{};
	GLuint m_vao{};
	GLuint m_vbo{};

	GLuint m_pointShader{};
	GLuint m_computeShader{};
};
