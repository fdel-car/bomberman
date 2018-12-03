#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <map>

class AudioManager {
   public:
	AudioManager(void);
	~AudioManager(void);

	void loadMusic(std::string filePath, std::string musicName);
	void loadSound(std::string filePath, std::string soundName);
	void clearMusics(std::vector<std::string> musicNames);
	void clearSounds(std::vector<std::string> soundNames);
	void playMusic(std::string musicName);
	void playSound(std::string soundName);

   private:
	AudioManager(AudioManager const &src);

	AudioManager &operator=(AudioManager const &rhs);

	sf::Sound _musicPlayer;
	sf::Sound _soundsPlayer;
	std::map<std::string, sf::SoundBuffer> _musicsMap;
	std::map<std::string, sf::SoundBuffer> _soundsMap;
};
