#include "engine/AGame.hpp"

AGame::AGame(size_t enumSize)
	: _spawnableEntities(std::vector<Entity *>()),
	  _entities(std::vector<Entity *>()),
	  _camera(nullptr),
	  _light(nullptr),
	  _skybox(nullptr),
	  _loadingEntities(std::vector<Entity *>()),
	  _loadingCamera(nullptr),
	  _loadingLight(nullptr),
	  _loadingSkybox(nullptr),
	  _neededFonts(std::vector<std::tuple<float, std::string, std::string>>()),
	  _allAssets(std::map<std::string, std::string>()),
	  _neededAssets(std::set<std::string>()),
	  _allSounds(std::map<std::string, std::string>()),
	  _neededSounds(std::set<std::string>()),
	  _collisionTable(std::vector<std::vector<bool>>(enumSize)) {
	for (auto &collisionTag : _collisionTable) {
		collisionTag = std::vector<bool>(enumSize, true);
	}
}

AGame::~AGame(void) {
	for (size_t idx = _loadingEntities.size() - 1;
		 idx < _loadingEntities.size(); idx--) {
		delete _loadingEntities[idx];
	}
	if (_loadingLight != nullptr) delete _loadingLight;
	if (_loadingSkybox != nullptr) delete _loadingSkybox;
	if (_loadingCamera != nullptr) delete _loadingCamera;
	_loadingEntities.clear();
}

std::vector<Entity *> const AGame::getEntities(void) const { return _entities; }

Camera *AGame::getCamera(void) const { return _camera; }

Light *AGame::getLight(void) const { return _light; }

Skybox *AGame::getSkybox(void) const { return _skybox; }

std::vector<Entity *> const AGame::getLoadingEntities(void) const {
	return _loadingEntities;
}

Camera *AGame::getLoadingCamera(void) const { return _loadingCamera; }

Light *AGame::getLoadingLight(void) const { return _loadingLight; }

Skybox *AGame::getLoadingSkybox(void) const { return _loadingSkybox; }

std::vector<glm::vec3> AGame::getInstancePosition() const {
	return _transforms;
}

std::vector<glm::vec3> AGame::getInstanceRotation() const {
	return _transformsRot;
}

std::vector<std::vector<bool>> const &AGame::getCollisionTable(void) {
	return _collisionTable;
}

std::vector<std::tuple<float, std::string, std::string>>
	&AGame::getNeededFont() {
	return _neededFonts;
}

void AGame::clearInstancePosition(void) { _transforms.clear(); }

void AGame::unload(void) {
	_spawnableEntities.clear();
	_entities.clear();

	_light = nullptr;
	_camera = nullptr;
	_skybox = nullptr;

	_neededAssets.clear();

	// Reset counter for next scene
	Entity::resetSpawnedEntities();
}

int AGame::getSceneIndexByName(std::string sceneName) const {
	int idx = 0;
	for (const auto &it : _scenesNames) {
		if (sceneName.compare(it) == 0) return idx;
		idx++;
	}
	return -1;
}

int AGame::getStartingMusicVolume(void) const { return 5; }

int AGame::getStartingSoundsVolume(void) const { return 10; }

void AGame::setLayerCollision(int layer1, int layer2, bool doCollide) {
	if ((size_t)layer1 >= _collisionTable.size() ||
		(size_t)layer2 >= _collisionTable.size()) {
		throw std::runtime_error(
			"\033[0;31m:Error:\033[0m Invalid layer for collision given !");
	}
	_collisionTable[layer1][layer2] = doCollide;
	_collisionTable[layer2][layer1] = doCollide;
}

void AGame::setGameRenderer(GameRenderer *gameRenderer) {
	_gameRenderer = gameRenderer;
	initAllAssets();
}

void AGame::setAudioManager(AudioManager *audioManager) {
	_audioManager = audioManager;
	initAllSounds();
}

void AGame::loadAssets(void) {
	std::map<std::string, std::string> neededAssets;
	std::map<std::string, std::string>::iterator it;

	for (auto assetName : _neededAssets) {
		// Skip if already added
		if (neededAssets.find(assetName) != neededAssets.end()) continue;

		// Retrieve file path
		it = _allAssets.find(assetName);
		if (it != _allAssets.end()) {
			neededAssets[assetName] = it->second;
		} else {
			std::cerr << "\033[0;33m:Warning:\033[0m Sound \"" << assetName
					  << "\" has not been added to _allAssets variable"
					  << std::endl;
		}
	}

	for (auto entity : _spawnableEntities) {
		// Skip if already added
		if (neededAssets.find(entity->getModelName()) != neededAssets.end())
			continue;

		// Retrieve file path
		it = _allAssets.find(entity->getModelName());
		if (it != _allAssets.end()) {
			neededAssets[entity->getModelName()] = it->second;
		} else {
			std::cerr << "\033[0;33m:Warning:\033[0m Sound \""
					  << entity->getModelName()
					  << "\" has not been added to _allAssets variable"
					  << std::endl;
		}
	}

	for (auto entity : _entities) {
		// Skip if already added
		if (neededAssets.find(entity->getModelName()) != neededAssets.end())
			continue;

		// Retrieve file path
		it = _allAssets.find(entity->getModelName());
		if (it != _allAssets.end()) {
			neededAssets[entity->getModelName()] = it->second;
		} else {
			std::cerr << "\033[0;33m:Warning:\033[0m Sound \""
					  << entity->getModelName()
					  << "\" has not been added to _allAssets variable"
					  << std::endl;
		}
	}

	_gameRenderer->loadAssets(neededAssets);
}

void AGame::loadSounds(void) {
	std::map<std::string, std::string> neededSounds;
	std::map<std::string, std::string>::iterator it;

	for (auto assetName : _neededSounds) {
		// Skip if already added
		if (neededSounds.find(assetName) != neededSounds.end()) continue;

		// Retrieve file path
		it = _allSounds.find(assetName);
		if (it != _allSounds.end()) {
			neededSounds[assetName] = it->second;
		} else {
			std::cerr << "\033[0;33m:Warning:\033[0m Sound \"" << assetName
					  << "\" has not been added to _allSounds variable"
					  << std::endl;
		}
	}

	if (_camera != nullptr) {
		for (auto sound : _camera->getNeededSounds()) {
			// Skip if already added
			if (neededSounds.find(sound) != neededSounds.end()) continue;

			// Retrieve file path
			it = _allSounds.find(sound);
			if (it != _allSounds.end()) {
				neededSounds[sound] = it->second;
			} else {
				std::cerr << "\033[0;33m:Warning:\033[0m Sound \"" << sound
						  << "\" has not been added to _allSounds variable"
						  << std::endl;
			}
		}
	}

	for (auto entity : _spawnableEntities) {
		for (auto sound : entity->getNeededSounds()) {
			// Skip if already added
			if (neededSounds.find(sound) != neededSounds.end()) continue;

			// Retrieve file path
			it = _allSounds.find(sound);
			if (it != _allSounds.end()) {
				neededSounds[sound] = it->second;
			} else {
				std::cerr << "\033[0;33m:Warning:\033[0m Sound \"" << sound
						  << "\" has not been added to _allSounds variable"
						  << std::endl;
			}
		}
	}

	for (auto entity : _entities) {
		for (auto sound : entity->getNeededSounds()) {
			// Skip if already added
			if (neededSounds.find(sound) != neededSounds.end()) continue;

			// Retrieve file path
			it = _allSounds.find(sound);
			if (it != _allSounds.end()) {
				neededSounds[sound] = it->second;
			} else {
				std::cerr << "\033[0;33m:Warning:\033[0m Sound \"" << sound
						  << "\" has not been added to _allSounds variable"
						  << std::endl;
			}
		}
	}

	_audioManager->loadSounds(neededSounds);
}
