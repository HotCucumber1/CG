#include "ParticleSystem.h"
#include <cmath>
#include <GL/glx.h>
#include <iostream>
#include <random>

// TODO нейминг и жестко разобраться во всем

const auto kernelSource = R"(
__kernel void updateParticles(
    __global float4* particles,
    const float deltaTime,
    const float G,
    const int numParticles,
    const float softening)
{
    int i = get_global_id(0);
    if (i >= numParticles)
	{
		return;
	}

    float4 pos_i = particles[i * 2];
    float4 vel_i = particles[i * 2 + 1];
    float mass_i = pos_i.w;

    float4 force = (float4)(0.0f, 0.0f, 0.0f, 0.0f);

    for (int j = 0; j < numParticles; j++)
	{
        if (i == j)
		{
			continue;
		}

        float4 pos_j = particles[j * 2];
        float mass_j = pos_j.w;

        float4 diff = pos_j - pos_i;
        float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z + softening;
        float dist = sqrt(distSq);

        float forceMag = (G * mass_i * mass_j) / distSq;
        float3 dir = (float3)(diff.x, diff.y, diff.z) / dist;

        force.x += dir.x * forceMag;
        force.y += dir.y * forceMag;
        force.z += dir.z * forceMag;
    }

    float4 acceleration = (float4)(force.x / mass_i, force.y / mass_i, force.z / mass_i, 0.0f);
    vel_i += acceleration * deltaTime;
    pos_i += vel_i * deltaTime;

    particles[i * 2] = pos_i;
    particles[i * 2 + 1] = vel_i;
})";

ParticleSystem::ParticleSystem(const int numParticles, const float G, const float timeScale)
	: m_numParticles(numParticles)
	, m_G(G)
	, m_timeScale(timeScale)
{
	m_particles.resize(m_maxParticles);
	SetupOpenCL();
	SetupOpenGLBuffers();
	CreateTexture();
}

ParticleSystem::~ParticleSystem()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_particleBufferGL);
	glDeleteTextures(1, &m_textureID);
}

void ParticleSystem::InitializeRandom(
	const float radius,
	const float maxMass,
	const float velScale)
{
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution dist01(0.0f, 1.0f);
	std::uniform_real_distribution distSym(-1.0f, 1.0f);

	for (int i = 0; i < m_numParticles; i++)
	{
		const auto theta = 2 * M_PI * dist01(rng);
		const auto phi = std::acos(2.0f * dist01(rng) - 1.0f);
		const auto r = radius * std::pow(dist01(rng), 1.0f / 3.0f);

		m_particles[i].position.x = r * std::sin(phi) * std::cos(theta);
		m_particles[i].position.y = r * std::sin(phi) * std::sin(theta);
		m_particles[i].position.z = r * std::cos(phi);
		m_particles[i].position.w = 0.5f + dist01(rng) * maxMass;

		m_particles[i].velocity.x = distSym(rng) * velScale;
		m_particles[i].velocity.y = distSym(rng) * velScale;
		m_particles[i].velocity.z = distSym(rng) * velScale;
		m_particles[i].velocity.w = 0.0f;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferGL);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_numParticles * sizeof(Particle), m_particles.data());
}

void ParticleSystem::Update(float deltaTime)
{
	deltaTime *= m_timeScale;
	try
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferGL);
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_numParticles * sizeof(Particle), m_particles.data());

		m_queue.enqueueWriteBuffer(
			m_particleBufferCL,
			CL_TRUE, 0,
			m_numParticles * sizeof(Particle),
			m_particles.data());

		m_kernel.setArg(0, m_particleBufferCL);
		m_kernel.setArg(1, deltaTime);
		m_kernel.setArg(2, m_G);
		m_kernel.setArg(3, m_numParticles);
		m_kernel.setArg(4, 0.1f);

		m_queue.enqueueNDRangeKernel(
			m_kernel,
			cl::NullRange,
			cl::NDRange(m_numParticles),
			cl::NullRange);

		m_queue.enqueueReadBuffer(
			m_particleBufferCL,
			CL_TRUE, 0,
			m_numParticles * sizeof(Particle),
			m_particles.data());

		glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferGL);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_numParticles * sizeof(Particle), m_particles.data());
	}
	catch (const std::exception& e)
	{
		std::cerr << "OpenCL update error: " << e.what() << std::endl;
	}
}

void ParticleSystem::Render() const
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	std::vector<glm::vec4> positions(m_numParticles);
	for (int i = 0; i < m_numParticles; i++)
	{
		positions[i] = m_particles[i].position;
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_numParticles * sizeof(glm::vec4), positions.data());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glDrawArrays(GL_POINTS, 0, m_numParticles);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBindVertexArray(0);
}

void ParticleSystem::AddParticle(const glm::vec3& position, float mass)
{
	if (m_numParticles >= m_maxParticles)
	{
		return;
	}

	m_particles[m_numParticles].position = glm::vec4(position, mass);
	m_particles[m_numParticles].velocity = glm::vec4(0, 0, 0, 0);
	m_numParticles++;

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferGL);
	glBufferSubData(GL_ARRAY_BUFFER, (m_numParticles - 1) * sizeof(Particle), sizeof(Particle), &m_particles[m_numParticles - 1]);
}

void ParticleSystem::SetupOpenCL()
{
	try
	{
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		const auto platform = platforms[0];

		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
		if (devices.empty())
		{
			platform.getDevices(CL_DEVICE_TYPE_CPU, &devices);
		}
		auto device = devices[0];

		m_context = cl::Context(device);
		m_queue = cl::CommandQueue(m_context, device);

		cl::Program::Sources sources;
		sources.emplace_back(kernelSource, strlen(kernelSource));
		m_program = cl::Program(m_context, sources);

		const std::string buildOptions = "-cl-std=CL1.2 -cl-fast-relaxed-math";
		if (m_program.build({ device }, buildOptions.c_str()) != CL_SUCCESS)
		{
			const auto buildLog = m_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
			std::cerr << "Build error: " << buildLog << std::endl;
			throw std::runtime_error("OpenCL kernel build failed");
		}

		m_kernel = cl::Kernel(m_program, "updateParticles");
	}
	catch (const std::exception& e)
	{
		std::cerr << "OpenCL Error: " << e.what() << std::endl;
		throw;
	}
}

void ParticleSystem::SetupOpenGLBuffers()
{
	glGenBuffers(1, &m_particleBufferGL);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleBufferGL);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 2 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);

	for (int i = 0; i < m_maxParticles; i++)
	{
		m_particles[i].position = glm::vec4(0, 0, 0, 1.0f);
		m_particles[i].velocity = glm::vec4(0, 0, 0, 0);
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_numParticles * sizeof(Particle), m_particles.data());

	m_particleBufferCL = cl::Buffer(m_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		m_numParticles * sizeof(Particle), m_particles.data());

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numParticles * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), static_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), reinterpret_cast<void*>(sizeof(glm::vec3)));

	glBindVertexArray(0);
}

void ParticleSystem::CreateTexture()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	unsigned char textureData[16 * 16 * 4];
	for (int i = 0; i < 16 * 16; i++)
	{
		const int x = i % 16;
		const int y = i / 16;
		const float dx = (x - 7.5f) / 7.5f;
		const float dy = (y - 7.5f) / 7.5f;
		const float dist = sqrt(dx * dx + dy * dy);

		if (dist < 1.0f)
		{
			float alpha = (1.0f - dist) * 255;
			textureData[i * 4 + 0] = 255;
			textureData[i * 4 + 1] = 200;
			textureData[i * 4 + 2] = 100;
			textureData[i * 4 + 3] = alpha;
		}
		else
		{
			textureData[i * 4 + 0] = 0;
			textureData[i * 4 + 1] = 0;
			textureData[i * 4 + 2] = 0;
			textureData[i * 4 + 3] = 0;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
