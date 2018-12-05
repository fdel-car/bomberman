#include "engine/AGame.hpp"

AGame::AGame(size_t enumSize)
	: _entities(std::vector<Entity *>()),
	  _camera(nullptr),
	  _light(nullptr),
	  _skybox(nullptr),
	  _loadingEntities(std::vector<Entity *>()),
	  _loadingCamera(nullptr),
	  _loadingLight(nullptr),
	  _loadingSkybox(nullptr) {
	_collisionTable = std::vector<std::vector<bool>>(enumSize);
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

std::vector<std::vector<bool>> const &AGame::getCollisionTable(void) {
	return _collisionTable;
}

std::vector<std::tuple<float, std::string, std::string>>
	&AGame::getNeededFont() {
	return _neededFonts;
}

void AGame::unload(void) {
	_entities.clear();

	_light = nullptr;
	_camera = nullptr;
	_skybox = nullptr;

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

void AGame::setAudioManager(AudioManager *audioManager) {
	_audioManager = audioManager;
}

void AGame::loadSounds(void) {
	std::map<std::string, std::string> neededSounds =
		std::map<std::string, std::string>();
	if (_camera != nullptr) {
		neededSounds = _camera->getNeededSounds();
	}

	for (auto entity : _entities) {
		for (auto sound : entity->getNeededSounds()) {
			neededSounds[sound.first] = sound.second;
		}
	}

	_audioManager->loadSounds(neededSounds);
}
