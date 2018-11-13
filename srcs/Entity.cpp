#include "Entity.hpp"
#include "GameEngine.hpp"

Entity::Entity(std::vector<float> position, std::vector<float> rotation,
			   Collider *collider)
	: _targetMovement(std::vector<float>(3, 0)),
	  _position(position),
	  _rotation(rotation),
	  _collider(collider) {}

Entity::~Entity(void) { delete _collider; }

void Entity::Update(void) {}

GameEngine *Entity::getGameEngine(void) const { return _gameEngine; }
const std::vector<float> &Entity::getPosition(void) const { return _position; }
const std::vector<float> &Entity::getRotation(void) const { return _rotation; }
const Collider *Entity::getCollider(void) const { return _collider; }
std::vector<float> &Entity::getTargetMovement(void) { return _targetMovement; }
void Entity::moveFromPosition(std::vector<float> &newPos) {
	_position[0] += newPos[0];
	_position[1] += newPos[1];
	_position[2] += newPos[2];
}

void Entity::setGameEngine(GameEngine *gameEngine) { _gameEngine = gameEngine; }
