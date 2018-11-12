#include "AEntity.hpp"
#include "GameEngine.hpp"

AEntity::AEntity(void)
	: position(std::vector<float>(3, 0)), rotation(std::vector<float>(4, 0)) {}

AEntity::AEntity(std::vector<float> position, std::vector<float> rotation)
	: position(position), rotation(rotation) {}

void AEntity::setGameEngine(GameEngine *gameEngine) {
	_gameEngine = gameEngine;
}
