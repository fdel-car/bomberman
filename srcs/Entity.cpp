#include "Entity.hpp"
#include "GameEngine.hpp"

Entity::Entity(glm::vec3 position, glm::vec3 rotation, Collider *collider,
			   Model *model, bool isTmp)
	: _position(position),
	  _rotation(rotation),
	  _collider(collider),
	  _model(model),
	  _isTmp(isTmp) {}

Entity::~Entity(void) {
	delete _collider;
	delete _model;
}

void Entity::Update(void) {}

GameEngine *Entity::getGameEngine(void) const { return _gameEngine; }
const glm::vec3 &Entity::getPosition(void) const { return _position; }
const glm::vec3 &Entity::getRotation(void) const { return _rotation; }
const Collider *Entity::getCollider(void) const { return _collider; }
const Model *Entity::getModel(void) const { return _model; }
bool Entity::getTmpState(void) const { return _isTmp; }

void Entity::setGameEngine(GameEngine *gameEngine) { _gameEngine = gameEngine; }
