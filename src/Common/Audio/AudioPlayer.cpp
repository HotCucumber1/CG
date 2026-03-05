#include "AudioPlayer.h"

#include <cstring>
#include <iostream>
#include <vector>

AudioPlayer::AudioPlayer()
{
	m_device = alcOpenDevice(nullptr);
	if (!m_device)
	{
		std::cerr << "Failed to open audio device" << std::endl;
		return;
	}

	m_context = alcCreateContext(m_device, nullptr);
	if (!m_context)
	{
		std::cerr << "Failed to create audio context" << std::endl;
		alcCloseDevice(m_device);
		return;
	}

	if (!alcMakeContextCurrent(m_context))
	{
		std::cerr << "Failed to make context current" << std::endl;
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
		return;
	}
}

AudioPlayer::~AudioPlayer()
{
	if (m_buffer)
	{
		alDeleteBuffers(1, &m_buffer);
	}
	if (m_source)
	{
		alDeleteSources(1, &m_source);
	}
	alcMakeContextCurrent(nullptr);
	if (m_context)
	{
		alcDestroyContext(m_context);
	}
	if (m_device)
	{
		alcCloseDevice(m_device);
	}
}

bool AudioPlayer::LoadWAV(const std::string& filename)
{
	constexpr int riffHeaderSize = 12;
	constexpr int chunkHeaderSize = 8;

	FILE* file = fopen(filename.c_str(), "rb");
	if (!file)
	{
		std::cerr << "Cannot open file: " << filename << std::endl;
		return false;
	}

	char riffHeader[riffHeaderSize];
	fread(riffHeader, 1, riffHeaderSize, file);

	if (memcmp(riffHeader, "RIFF", 4) != 0 || memcmp(riffHeader + chunkHeaderSize, "WAVE", 4) != 0)
	{
		std::cerr << "Invalid WAV file (not RIFF/WAVE format)" << std::endl;
		fclose(file);
		return false;
	}

	char chunkHeader[chunkHeaderSize];
	bool foundFormat = false;
	short channels = 0;
	int sampleRate = 0;
	short bitsPerSample = 0;

	while (fread(chunkHeader, 1, chunkHeaderSize, file) == chunkHeaderSize)
	{
		if (memcmp(chunkHeader, "fmt ", 4) == 0)
		{
			const int chunkSize = *reinterpret_cast<int*>(chunkHeader + 4);

			std::vector<char> formatData(chunkSize);
			fread(formatData.data(), 1, chunkSize, file);

			channels = *reinterpret_cast<short*>(&formatData[2]);
			sampleRate = *reinterpret_cast<int*>(&formatData[4]);
			bitsPerSample = *reinterpret_cast<short*>(&formatData[14]);

			foundFormat = true;
		}
		else if (memcmp(chunkHeader, "data", 4) == 0)
		{
			const int dataSize = *reinterpret_cast<int*>(chunkHeader + 4);

			std::vector<char> data(dataSize);
			fread(data.data(), 1, dataSize, file);

			if (!foundFormat)
			{
				std::cerr << "Found data before fmt chunk" << std::endl;
				fclose(file);
				return false;
			}
			fclose(file);

			ALenum format;
			if (channels == 1 && bitsPerSample == 16)
			{
				format = AL_FORMAT_MONO16;
			}
			else if (channels == 2 && bitsPerSample == 16)
			{
				format = AL_FORMAT_STEREO16;
			}
			else if (channels == 1 && bitsPerSample == 8)
			{
				format = AL_FORMAT_MONO8;
			}
			else if (channels == 2 && bitsPerSample == 8)
			{
				format = AL_FORMAT_STEREO8;
			}
			else
			{
				std::cerr << "Unsupported format: " << channels << "ch, " << bitsPerSample << "bit" << std::endl;
				return false;
			}

			if (m_buffer)
			{
				alDeleteBuffers(1, &m_buffer);
			}
			if (m_source)
			{
				alDeleteSources(1, &m_source);
			}

			alGenBuffers(1, &m_buffer);
			alBufferData(m_buffer, format, data.data(), dataSize, sampleRate);

			alGenSources(1, &m_source);
			alSourcei(m_source, AL_BUFFER, m_buffer);
			alSourcef(m_source, AL_GAIN, 1.0f);

			return true;
		}
		else
		{
			const int chunkSize = *reinterpret_cast<int*>(chunkHeader + 4);
			fseek(file, chunkSize, SEEK_CUR);
		}
	}

	std::cerr << "No data chunk found in WAV file" << std::endl;
	fclose(file);
	return false;
}

void AudioPlayer::Play() const
{
	if (!m_source)
	{
		std::cerr << "Cannot play: no sound loaded" << std::endl;
		return;
	}
	alSourcePlay(m_source);
}

bool AudioPlayer::CheckError(const std::string& operation)
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << "OpenAL error during " << operation << ": ";
		switch (error)
		{
		case AL_INVALID_NAME:
			std::cerr << "AL_INVALID_NAME";
			break;
		case AL_INVALID_ENUM:
			std::cerr << "AL_INVALID_ENUM";
			break;
		case AL_INVALID_VALUE:
			std::cerr << "AL_INVALID_VALUE";
			break;
		case AL_INVALID_OPERATION:
			std::cerr << "AL_INVALID_OPERATION";
			break;
		case AL_OUT_OF_MEMORY:
			std::cerr << "AL_OUT_OF_MEMORY";
			break;
		default:
			std::cerr << "Unknown error (" << error << ")";
			break;
		}
		std::cerr << std::endl;
		return false;
	}
	return true;
}