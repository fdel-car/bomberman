#include "AGameScene.hpp"

AGameScene::AGameScene(void) : _camera(NULL) {}

std::vector<Entity *> const AGameScene::getEntities(void) const {
	return _entities;
}

Camera *AGameScene::getCamera(void) const { return _camera; }