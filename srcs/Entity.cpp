#include "Entity.hpp"
#include "GameEngine.hpp"

Entity::Entity(std::vector<float> position, std::vector<float> rotation,
			   Collider collider)
	: _position(position), _rotation(rotation), _collider(collider) {}

void Entity::Update(void) {}

GameEngine *Entity::getGameEngine(void) const { return _gameEngine; }
const std::vector<float> &Entity::getPosition(void) const { return _position; }
const std::vector<float> &Entity::getRotation(void) const { return _rotation; }
const Collider &Entity::getCollider(void) const { return _collider; };

void Entity::setGameEngine(GameEngine *gameEngine) { _gameEngine = gameEngine; }
