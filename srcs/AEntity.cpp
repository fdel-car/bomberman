#include "AEntity.hpp"
#include "GameEngine.hpp"

AEntity::AEntity(void)
	: position(std::vector<float>(3, 0)),
	  rotation(std::vector<float>(4, 0)),
	  collider(Collider()) {}

AEntity::AEntity(std::vector<float> position, std::vector<float> rotation,
				 Collider collider)
	: position(position), rotation(rotation), collider(collider) {}

void AEntity::setGameEngine(GameEngine *gameEngine) {
	_gameEngine = gameEngine;
}
