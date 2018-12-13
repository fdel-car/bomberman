#include "engine/AudioManager.hpp"

extern std::string _assetsDir;

AudioManager::AudioManager(int musicVolume, int soundsVolume)
	: _musicVolume(musicVolume), _soundsVolume(soundsVolume) {
	_musicPlayer.setLoop(true);
	_musicPlayer.setVolume(_musicVolume);
}

AudioManager::~AudioManager(void) {}

void AudioManager::updateMusicVolume(int newValue) {
	_musicVolume = newValue;
	_musicPlayer.setVolume(_musicVolume * 0.5f);
}
void AudioManager::updateSoundsVolume(int newValue) {
	_soundsVolume = newValue;
	for (auto player : _soundPlayers) {
		player.setVolume(_soundsVolume * 100.0f);
	}
}

void AudioManager::loadSounds(std::map<std::string, std::string> resources) {
	// Stop all sounds
	for (auto player : _soundPlayers) {
		player.stop();
	}

	// Remove old srcs if no longer needed
	std::vector<std::string> toDelete;
	for (auto &elem : _soundsMap) {
		if (resources.find(elem.first) == resources.end()) {
			toDelete.push_back(elem.first);
		}
	}
	for (auto name : toDelete) {
		_soundsMap.erase(name);
	}

	// Add new
	for (auto resource : resources) {
		if (_soundsMap.find(resource.first) == _soundsMap.end()) {
			_soundsMap[resource.first] = sf::SoundBuffer();
			if (!_soundsMap[resource.first].loadFromFile(_assetsDir +
														 resource.second)) {
				std::cerr
					<< "\033[0;33m:Warning:\033[0m Could not load Sound at "
					<< resource.second << std::endl;
			}
		}
	}
}

void AudioManager::playMusic(std::string musicPath) {
	if (musicPath.compare("") == 0) {
		_musicPlayer.pause();
		return;
	}

	if (_musicPlayer.openFromFile(_assetsDir + musicPath)) {
		_musicPlayer.setVolume(_musicVolume);
		_musicPlayer.play();
	} else {
		std::cerr << "\033[0;33m:Warning:\033[0m Could not load Music at "
				  << musicPath << std::endl;
	}
}

void AudioManager::playSound(std::string soundName) {
	if (_soundsMap.find(soundName) != _soundsMap.end()) {
		// Delete sounds that have finished
		for (size_t i = _soundPlayers.size() - 1; i < _soundPlayers.size();
			 i--) {
			if (_soundPlayers[i].getStatus() != _soundPlayers[i].Playing) {
				_soundPlayers.erase(_soundPlayers.begin() + i);
			}
		}

		// SFML limit: cannot have more than 256 music/sound players
		if (_soundPlayers.size() < 254) {
			_soundPlayers.push_back(sf::Sound());
			_soundPlayers.back().setVolume(_soundsVolume);
			_soundPlayers.back().setBuffer(_soundsMap[soundName]);
			_soundPlayers.back().play();
		} else {
			std::cerr
				<< "\033[0;33m:Warning:\033[0m To many sounds playing at the "
				   "same time, wait for them to end before starting new ones"
				<< std::endl;
		}
	} else {
		std::cerr << "\033[0;33m:Warning:\033[0m Trying to play a sound that "
					 "has not been loaded! ("
				  << soundName << ")" << std::endl;
	}
}