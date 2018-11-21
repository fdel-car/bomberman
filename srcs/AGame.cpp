#include "AGame.hpp"

AGame::AGame(void) : _camera(nullptr) {}

AGame::~AGame(void) { unload(); }

std::vector<Entity *> const AGame::getEntities(void) const { return _entities; }

Camera *AGame::getCamera(void) const { return _camera; }

std::vector<std::tuple<float, std::string, std::string>>
	&AGame::getNeededFont() {
	return _neededFonts;
}

void AGame::unload(void) {
	if (_camera != nullptr) {
		delete _camera;
		_camera = nullptr;
	}
	for (auto entity : _entities) delete entity;
	_entities.clear();

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
