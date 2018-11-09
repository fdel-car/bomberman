#pragma once

#include <SFML/Audio.hpp>

class AudioManager
{
  private:
	AudioManager(AudioManager const &src);

	AudioManager &operator=(AudioManager const &rhs);

	sf::SoundBuffer startBuffer;
	sf::SoundBuffer eatBuffer;
	sf::SoundBuffer deathBuffer;
	sf::Sound sound;

  public:
	AudioManager(void);
	~AudioManager(void);

	void playStartSound(void);
	void playEatSound(void);
	void playDeathSound(void);

	bool active;
};
