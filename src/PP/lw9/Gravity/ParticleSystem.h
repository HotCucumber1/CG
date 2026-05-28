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

	void AddParticle(const glm::vec3& position, float mass);

	int GetParticleCount() const
	{
		return m_numParticles;
	}

	void SetGravitationalConstant(float G)
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
	void SetupOpenCL();
	void SetupOpenGLBuffers();
	void CreateTexture();

	std::vector<Particle> m_particles;
	int m_numParticles;
	int m_maxParticles = 100000;
	float m_G;
	float m_timeScale;

	cl::Context m_context;
	cl::CommandQueue m_queue;
	cl::Program m_program;
	cl::Kernel m_kernel;
	cl::Buffer m_particleBufferCL;

	GLuint m_particleBufferGL{};
	GLuint m_textureID{};
	GLuint m_vao{};
	GLuint m_vbo{};

	GLuint m_pointShader{};
	GLuint m_computeShader{};
};
