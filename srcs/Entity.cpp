#include "Entity.hpp"
#include "GameEngine.hpp"

Entity::Entity(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
			   Shape *shape, bool isTmp)
	: _position(position),
	  _modelMatrix(glm::mat4(1.0f)),
	  _collider(collider),
	  _shape(shape),
	  _isTmp(isTmp) {
	glm::quat rotationQuat = glm::quat(eulerAngles);
	_modelMatrix = glm::translate(_modelMatrix, position);
	_modelMatrix =
		glm::rotate(_modelMatrix, rotationQuat.w,
					glm::vec3(rotationQuat.x, rotationQuat.y, rotationQuat.z));
}

Entity::~Entity(void) {
	if (_collider) delete _collider;
}

void Entity::Update(void) {}

GameEngine *Entity::getGameEngine(void) const { return _gameEngine; }
const glm::vec3 &Entity::getPosition(void) const { return _position; }
const glm::mat4 &Entity::getModelMatrix(void) const { return _modelMatrix; }
const Collider *Entity::getCollider(void) const { return _collider; }
const Shape *Entity::getShape(void) const { return _shape; }
bool Entity::getTmpState(void) const { return _isTmp; }

void Entity::translate(glm::vec3 translation) {
	_modelMatrix = glm::translate(_modelMatrix, translation);
	_position += translation;
}

void Entity::rotate(glm::vec3 axis, float angle) {
	float length = glm::length(axis);
	if (1.0 > length + FLT_EPSILON ||
		1.0 < length - FLT_EPSILON)  // Normalize if needed
		axis = glm::normalize(axis);
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(angle), axis);
}

void Entity::rotateY(float angle) {
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(angle),
							   glm::vec3(0.0, 1.0, 0.0));
}

void Entity::setGameEngine(GameEngine *gameEngine) { _gameEngine = gameEngine; }
