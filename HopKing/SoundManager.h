#pragma once
#include <map>

class SoundEffect;
class SoundStream;

class SoundManager final
{
public:
	static SoundManager* Get();
	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;


	void Initialize();
	void AddSFX(std::string filePath);
	void AddSoundStream(std::string filePath);
	void PlaySFX(std::string filePath, int loops);
	void PlaySoundStream(std::string filePath, bool repeat);
	void StopSoundStream(std::string filePath);


protected:
	std::map<std::string, SoundEffect*> m_SFX;
	std::map<std::string, SoundStream*> m_SoundStreams;
};

