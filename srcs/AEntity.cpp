#include "AEntity.hpp"
#include "GameLogic.hpp"

AEntity::AEntity(void)
	: position(std::vector<float>(3, 0)), rotation(std::vector<float>(4, 0)) {}

AEntity::AEntity(GameLogic *_gameLogic, std::vector<float> position,
				 std::vector<float> rotation)
	: position(position), rotation(rotation), _gameLogic(_gameLogic) {}
