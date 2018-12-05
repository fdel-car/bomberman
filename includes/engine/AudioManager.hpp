#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

class AudioManager final {
   public:
	AudioManager(int musicVolume, int soundsVolume);
	~AudioManager(void);

	void updateMusicVolume(int newValue);
	void updateSoundsVolume(int newValue);
	void loadSounds(std::map<std::string, std::string> sounds);
	void playMusic(std::string musicPath);
	void playSound(std::string soundName);

   private:
	AudioManager(void);
	AudioManager(AudioManager const &src);

	AudioManager &operator=(AudioManager const &rhs);

	int _musicVolume;
	int _soundsVolume;
	sf::Music _musicPlayer;
	std::vector<sf::Sound> _soundPlayers;
	std::map<std::string, sf::SoundBuffer> _soundsMap;
};
