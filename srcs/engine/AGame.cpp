#include "engine/AGame.hpp"

AGame::AGame(size_t enumSize) : _camera(nullptr) {
	_collisionTable = std::vector<std::vector<bool>>(enumSize);
	for (auto &collisionTag : _collisionTable) {
		collisionTag = std::vector<bool>(enumSize, true);
	}
}

AGame::~AGame(void) {}

std::vector<Entity *> const AGame::getEntities(void) const { return _entities; }

Camera *AGame::getCamera(void) const { return _camera; }

Light *AGame::getLight(void) const { return _light; }

std::vector<std::vector<bool>> const &AGame::getCollisionTable(void) {
	return _collisionTable;
}

std::vector<std::tuple<float, std::string, std::string>>
	&AGame::getNeededFont() {
	return _neededFonts;
}

void AGame::unload(void) {
	// for (auto entity : _entities) delete entity;
	_entities.clear();

	// if (_camera != nullptr) {
	// 	delete _camera;
	// 	_camera = nullptr;
	// }

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
	_collisionTable[layer1][layer2] = doCollide;
	_collisionTable[layer2][layer1] = doCollide;
}
