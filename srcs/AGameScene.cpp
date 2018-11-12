#include "AGameScene.hpp"

AGameScene::AGameScene(void) : startEntities(std::vector<AEntity *>()) {}

AGameScene::AGameScene(AGameScene const &src) { *this = src; }

AGameScene::~AGameScene(void) {
	for (auto entity : startEntities) {
		delete entity;
	}
}

AGameScene &AGameScene::operator=(AGameScene const &rhs) {
	return *this;
	(void)rhs;
}