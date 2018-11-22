#pragma once

#include <SFML/Audio.hpp>

class AudioManager {
   public:
	AudioManager(void);
	~AudioManager(void);

   private:
	AudioManager(AudioManager const &src);

	AudioManager &operator=(AudioManager const &rhs);

	// sf::SoundBuffer _buffer;
	// sf::Sound _sound;
};
