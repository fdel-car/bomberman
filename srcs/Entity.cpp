#include "Entity.hpp"
#include "GameEngine.hpp"

Entity::Entity(GameEngine *_gameEngine, std::vector<float> position,
			   std::vector<float> rotation)
	: _position(position), _rotation(rotation), _gameEngine(_gameEngine) {}

void Entity::Update(void) {}

GameEngine *Entity::getGameEngine(void) const { return _gameEngine; }
const std::vector<float> &Entity::getPosition(void) const { return _position; }
const std::vector<float> &Entity::getRotation(void) const { return _rotation; }
