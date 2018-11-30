#include "engine/AudioManager.hpp"

AudioManager::AudioManager(void)
	: _musicsMap(std::map<std::string, sf::SoundBuffer>()),
	  _soundsMap(std::map<std::string, sf::SoundBuffer>()) {}

AudioManager::~AudioManager(void) {}

void AudioManager::loadMusic(std::string filePath, std::string musicName) {
	if (_musicsMap.find(musicName) == _musicsMap.end()) {
		_musicsMap[musicName] = sf::SoundBuffer();
		if (!_musicsMap[musicName].loadFromFile(filePath)) {
			std::cerr << "\033[0;33m:Warning:\033[0m Could not load Music"
					  << std::endl;
		}
	}
}
void AudioManager::loadSound(std::string filePath, std::string soundName) {
	if (_soundsMap.find(soundName) == _soundsMap.end()) {
		_soundsMap[soundName] = sf::SoundBuffer();
		if (!_soundsMap[soundName].loadFromFile(filePath)) {
			std::cerr << "\033[0;33m:Warning:\033[0m There is no light in the "
						 "loaded scene, you should definitely add one"
					  << std::endl;
		}
	}
}

void AudioManager::clearMusics(std::vector<std::string> musicNames) {
	for (auto musicName : musicNames) {
		if (_musicsMap.find(musicName) == _musicsMap.end()) {
			_musicsMap.erase(musicName);
		}
	}
}
void AudioManager::clearSounds(std::vector<std::string> soundNames) {
	for (auto soundName : soundNames) {
		if (_soundsMap.find(soundName) == _soundsMap.end()) {
			_soundsMap.erase(soundName);
		}
	}
}

void AudioManager::playMusic(std::string musicName) {
	if (_musicsMap.find(musicName) == _musicsMap.end()) {
		_musicPlayer.setBuffer(_musicsMap[musicName]);
		_musicPlayer.play();
	}
}
void AudioManager::playSound(std::string soundName) {
	if (_soundsMap.find(soundName) == _soundsMap.end()) {
		_soundsPlayer.setBuffer(_soundsMap[soundName]);
		_soundsPlayer.play();
	}
}