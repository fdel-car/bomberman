#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

class AudioManager {
   public:
	AudioManager(void);
	~AudioManager(void);

	void loadSounds(std::map<std::string, std::string> sounds);
	void playMusic(std::string musicPath);
	void playSound(std::string soundName);

   private:
	AudioManager(AudioManager const &src);

	AudioManager &operator=(AudioManager const &rhs);

	sf::Music _musicPlayer;
	std::vector<sf::Sound> _soundPlayers;
	std::map<std::string, sf::SoundBuffer> _soundsMap;
};
