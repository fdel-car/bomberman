#include "AudioManager.hpp"

// === CONSTRUCTOR =============================================================

AudioManager::AudioManager(void)
{
	active = false;

	// if (!startBuffer.loadFromFile("sounds/start.wav"))
	// {
	// 	return;
	// }
	// if (!eatBuffer.loadFromFile("sounds/eat.wav"))
	// {
	// 	return;
	// }
	// if (!deathBuffer.loadFromFile("sounds/death.wav"))
	// {
	// 	return;
	// }

	active = true;
	return;
}

AudioManager::AudioManager(AudioManager const &src)
{
	*this = src;
	return;
}

AudioManager::~AudioManager(void)
{
	if (!active)
		return;
	return;
}

// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================

AudioManager &AudioManager::operator=(AudioManager const &rhs)
{
	this->active = rhs.active;
	return *this;
}

// === ENDOPERATORS ============================================================

// === PRIVATE FUNCS ===========================================================
// === END PRIVATE FUNCS =======================================================

// === OVERRIDES ===============================================================
void AudioManager::play_start_sound(void)
{
	if (!active)
		return;
	// sound.setBuffer(startBuffer);
	// sound.play();
	return;
}

void AudioManager::play_eat_sound(void)
{
	if (!active)
		return;
	// sound.setBuffer(eatBuffer);
	// sound.play();
	return;
}

void AudioManager::play_death_sound(void)
{
	if (!active)
		return;
	// sound.setBuffer(deathBuffer);
	// sound.play();
	return;
}

// === END OVERRIDES ===========================================================
