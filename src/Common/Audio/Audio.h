#pragma once
#include <string>
#include <AL/al.h>
#include <AL/alc.h>

class Audio
{
public:
	Audio();
	~Audio();

	bool LoadWAV(const std::string& filename);

	void Play() const;

	static bool CheckError(const std::string& operation) ;

private:
	ALCdevice* m_device = nullptr;
	ALCcontext* m_context = nullptr;
	ALuint m_buffer = 0;
	ALuint m_source = 0;
};
