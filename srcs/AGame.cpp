#include "AGame.hpp"

AGame::AGame(void) : _camera(nullptr) {}

AGame::~AGame(void) { unload(); }

std::vector<Entity *> const AGame::getEntities(void) const { return _entities; }

Camera *AGame::getCamera(void) const { return _camera; }

std::vector<std::tuple<float, std::string, std::string>>
	&AGame::getNeededFont() {
	return vNeededFont;
}

void AGame::unload(void) {
	if (_camera != nullptr) {
		delete _camera;
		_camera = nullptr;
	}
	for (auto entity : _entities) delete entity;
	_entities.clear();
}
