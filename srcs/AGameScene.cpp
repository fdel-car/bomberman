#include "AGameScene.hpp"

AGameScene::AGameScene(void) : _camera(NULL) {}

SceneData const AGameScene::getData(void) const { return _vector; }
Camera *AGameScene::getCamera(void) const { return _camera; }