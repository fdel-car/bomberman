#include "engine/AudioManager.hpp"

AudioManager::AudioManager(void)
	: _musicsMap(std::map<std::string, sf::SoundBuffer>()),
	  _soundsMap(std::map<std::string, sf::SoundBuffer>()) {}

AudioManager::~AudioManager(void) {}

void AudioManager::loadMusics(std::map<std::string, std::string> resources) {
	// Remove old if no longer needed
	std::string fileName;
	std::vector<std::string> toDelete;
	for (auto &elem : _musicsMap) {
		fileName = std::get<0>(elem);
		if (resources.find(fileName) == resources.end()) {
			toDelete.push_back(fileName);
		}
	}
	for (auto name : toDelete) {
		_musicsMap.erase(name);
	}

	// Add new
	for (auto resource : resources) {
		if (_musicsMap.find(std::get<0>(resource)) == _musicsMap.end()) {
			_musicsMap[std::get<0>(resource)] = sf::SoundBuffer();
			if (!_musicsMap[std::get<0>(resource)].loadFromFile(
					std::get<1>(resource))) {
				std::cerr
					<< "\033[0;33m:Warning:\033[0m Could not load Music at "
					<< std::get<1>(resource) << std::endl;
			}
		}
	}
}
void AudioManager::loadSounds(std::map<std::string, std::string> resources) {
	// Remove old if no longer needed
	std::string fileName;
	std::vector<std::string> toDelete;
	for (auto &elem : _soundsMap) {
		fileName = std::get<0>(elem);
		if (resources.find(fileName) == resources.end()) {
			toDelete.push_back(fileName);
		}
	}
	for (auto name : toDelete) {
		_soundsMap.erase(name);
	}

	// Add new
	for (auto resource : resources) {
		if (_soundsMap.find(std::get<0>(resource)) == _soundsMap.end()) {
			_soundsMap[std::get<0>(resource)] = sf::SoundBuffer();
			if (!_soundsMap[std::get<0>(resource)].loadFromFile(
					std::get<1>(resource))) {
				std::cerr
					<< "\033[0;33m:Warning:\033[0m Could not load Sound at "
					<< std::get<1>(resource) << std::endl;
			}
		}
	}
}

void AudioManager::playMusic(std::string musicName) {
	if (_musicsMap.find(musicName) != _musicsMap.end()) {
		_musicPlayer.setBuffer(_musicsMap[musicName]);
		_musicPlayer.play();
	}
}
void AudioManager::playSound(std::string soundName) {
	if (_soundsMap.find(soundName) != _soundsMap.end()) {
		_soundsPlayer.setBuffer(_soundsMap[soundName]);
		_soundsPlayer.play();
	}
}