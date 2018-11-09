#include "AudioManager.hpp"

AudioManager::AudioManager(void)
{
	active = false;

	if (!startBuffer.loadFromFile("sounds/start.wav"))
	{
		return;
	}
	if (!eatBuffer.loadFromFile("sounds/eat.wav"))
	{
		return;
	}
	if (!deathBuffer.loadFromFile("sounds/death.wav"))
	{
		return;
	}

	active = true;
}

AudioManager::AudioManager(AudioManager const &src)
{
	*this = src;
}

AudioManager::~AudioManager(void)
{
}

AudioManager &AudioManager::operator=(AudioManager const &rhs)
{
	this->active = rhs.active;
	return *this;
}

void AudioManager::playStartSound(void)
{
	if (!active)
		return;
	sound.setBuffer(startBuffer);
	sound.play();
}

void AudioManager::playEatSound(void)
{
	if (!active)
		return;
	sound.setBuffer(eatBuffer);
	sound.play();
}

void AudioManager::playDeathSound(void)
{
	if (!active)
		return;
	sound.setBuffer(deathBuffer);
	sound.play();
}