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

	void loadMusics(std::map<std::string, std::string> musics);
	void loadSounds(std::map<std::string, std::string> sounds);
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
