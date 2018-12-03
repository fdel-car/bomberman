#include "engine/AGame.hpp"

AGame::AGame(size_t enumSize)
	: _camera(nullptr),
	  _light(nullptr),
	  // _skybox(nullptr),
	  _neededMusic(std::vector<std::tuple<std::string, std::string>>()),
	  _neededSounds(std::vector<std::tuple<std::string, std::string>>()) {
	_collisionTable = std::vector<std::vector<bool>>(enumSize);
	for (auto &collisionTag : _collisionTable) {
		collisionTag = std::vector<bool>(enumSize, true);
	}
}

AGame::~AGame(void) {}

std::vector<Entity *> const AGame::getEntities(void) const { return _entities; }

Camera *AGame::getCamera(void) const { return _camera; }

Light *AGame::getLight(void) const { return _light; }

// Skybox *AGame::getSkybox(void) const { return _skybox; }

const std::vector<std::tuple<std::string, std::string>>
	&AGame::getNeeededMusic() const {
	return _neededMusic;
}
const std::vector<std::tuple<std::string, std::string>>
	&AGame::getNeeededSounds() const {
	return _neededSounds;
}

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

void AGame::setLayerCollision(int layer1, int layer2, bool doCollide) {
	if ((size_t)layer1 >= _collisionTable.size() ||
		(size_t)layer2 >= _collisionTable.size()) {
		throw std::runtime_error(
			"\033[0;31m:Error:\033[0m Invalid layer for collision given !");
	}
	_collisionTable[layer1][layer2] = doCollide;
	_collisionTable[layer2][layer1] = doCollide;
}
