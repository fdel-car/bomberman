#include "Entity.hpp"
#include "GameEngine.hpp"

Entity::Entity(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
			   Shape *shape, bool isTmp)
	: _position(position),
	  _modelMatrix(glm::mat4(1.0f)),
	  _collider(collider),
	  _shape(shape),
	  _isTmp(isTmp) {
	// std::cout << "Euler Angles: " << eulerAngles.x << ' ' << eulerAngles.y
	// 		  << ' ' << eulerAngles.z << std::endl;

	// float c1 = cos(glm::radians(eulerAngles.z) / 2);
	// float s1 = sin(glm::radians(eulerAngles.z) / 2);
	// float c2 = cos(eulerAngles.y / 2);
	// float s2 = sin(eulerAngles.y / 2);
	// float c3 = cos(eulerAngles.x / 2);
	// float s3 = sin(eulerAngles.x / 2);
	// float c1c2 = c1 * c2;
	// float s1s2 = s1 * s2;
	// glm::quat rotQuaternion(c1c2 * c3 - s1s2 * s3, c1c2 * s3 + s1s2 * c3,
	// 						s1 * c2 * c3 + c1 * s2 * s3,
	// 						c1 * s2 * c3 - s1 * c2 * s3);

	// std::cout << "Quaternion: " << rotQuaternion.x << ' ' << rotQuaternion.y
	// 		  << ' ' << rotQuaternion.z << " | " << rotQuaternion.w
	// 		  << std::endl;

	// glm::quat tmp(eulerAngles);

	// std::cout << "Tmp quaternion: " << tmp.x << ' ' << tmp.y << ' ' << tmp.z
	// 		  << " | " << tmp.w << std::endl;

	// std::cout << glm::radians(45.0f) << std::endl;

	_modelMatrix = glm::translate(_modelMatrix, position);
	// _modelMatrix =
	// glm::rotate(_modelMatrix, rotQuaternion.w, glm::vec3(0.0, 0.0, 1.0));
	(void)eulerAngles;
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
