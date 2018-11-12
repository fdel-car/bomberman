#include "AEntity.hpp"
#include "GameEngine.hpp"

AEntity::AEntity(void)
	: position(std::vector<float>(3, 0)), rotation(std::vector<float>(4, 0)) {}

AEntity::AEntity(GameEngine *_gameEngine, std::vector<float> position,
				 std::vector<float> rotation)
	: position(position), rotation(rotation), _gameEngine(_gameEngine) {}
