#include "pch.h"
#include "SoundManager.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include <algorithm>


SoundManager* SoundManager::Get() {
	static SoundManager* s = new SoundManager();

	return s;
}

SoundManager::SoundManager()
	: m_SFX{}
	, m_SoundStreams{}
{
}

SoundManager::~SoundManager()
{
	for (std::pair<std::string, SoundEffect*> sf : m_SFX) {
		delete sf.second;
		sf.second = nullptr;
	}

	m_SFX.clear();

	for (std::pair<std::string, SoundStream*> ss : m_SoundStreams) {
		delete ss.second;
		ss.second = nullptr;
	}

	m_SoundStreams.clear();
}

void SoundManager::Initialize()
{
	AddSoundStream("forestMusic.mp3");
	AddSoundStream("blackholeMusic.mp3");
}

void SoundManager::AddSFX(std::string filePath)
{
	m_SFX.insert(std::make_pair(filePath, new SoundEffect("Resources/Sounds/" + filePath)));
}

void SoundManager::AddSoundStream(std::string filePath)
{
	if (!m_SoundStreams.contains(filePath))
	{
		m_SoundStreams.insert(std::make_pair(filePath, new SoundStream("Resources/Sounds/" + filePath)));
	}
}

void SoundManager::PlaySFX(std::string filePath, int loops)
{
	m_SFX[filePath]->Play(loops);
}

void SoundManager::PlaySoundStream(std::string filePath, bool repeat)
{
	for (std::pair<std::string, SoundStream*> ss : m_SoundStreams) {
		if (ss.second->IsPlaying() && ss.first != filePath)
		{
			ss.second->Stop();
		}
	}

	if (!m_SoundStreams[filePath]->IsPlaying())
	{
		m_SoundStreams[filePath]->Play(repeat);
	}
}

void SoundManager::StopSoundStream(std::string filePath)
{
	m_SoundStreams[filePath]->Stop();
}

