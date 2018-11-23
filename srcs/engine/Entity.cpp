#include "engine/Entity.hpp"
#include "engine/Camera.hpp"
#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"

size_t Entity::_spawnedEntities = 0;

void Entity::resetSpawnedEntities(void) { _spawnedEntities = 0; }

Entity::Entity(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
			   std::string modelName, std::string name, std::string tag,
			   Entity *sceneManager)
	: _position(position),
	  _modelMatrix(glm::mat4(1.0f)),
	  _id(Entity::_spawnedEntities++),
	  _needToBeDestroyed(false),
	  _modelName(modelName),
	  _name(name),
	  _tag(tag),
	  _sceneManager(sceneManager),
	  _collider(collider),
	  _isTmp(true),
	  _targetMovement(glm::vec3()) {
	_rotation = glm::quat(glm::radians(eulerAngles));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	_modelMatrix = translationMatrix * glm::mat4_cast(_rotation);
	_name = _modelName;
	_gameEngine = nullptr;

	// Signal, if _sceneManager is set, where is entity starting pos
	if (_sceneManager != nullptr) _sceneManager->tellPosition(this);
}

Entity::~Entity(void) {
	// Signal, if _sceneManager is set, that entity will be destroyed
	if (_sceneManager != nullptr) {
		_sceneManager->tellDestruction(this);
	}
	if (_collider) delete _collider;
}

void Entity::update(void) {}

void Entity::tellPosition(Entity *entity) { (void)entity; }

void Entity::tellDestruction(Entity *entity) { (void)entity; }

GameEngine *Entity::getGameEngine(void) const { return _gameEngine; }

const glm::vec3 &Entity::getPosition(void) const { return _position; }

const glm::mat4 &Entity::getModelMatrix(void) const { return _modelMatrix; }

const Collider *Entity::getCollider(void) const { return _collider; }

const Model *Entity::getModel(void) const { return _model; }

bool Entity::getTmpState(void) const { return _isTmp; }

size_t const &Entity::getId(void) const { return _id; }

std::string const &Entity::getName(void) const { return _name; }

std::string const &Entity::getTag(void) const { return _tag; }

glm::vec3 &Entity::getTargetMovement(void) { return _targetMovement; }

bool Entity::getNeedToBeDestroyed(void) const { return _needToBeDestroyed; }

void Entity::translate(glm::vec3 translation) {
	_modelMatrix[3][0] += translation.x;
	_modelMatrix[3][1] += translation.y;
	_modelMatrix[3][2] += translation.z;
	_position += translation;

	// Signal, if _sceneManager is set, where is entity new pos
	if (_sceneManager != nullptr) _sceneManager->tellPosition(this);
}

glm::vec3 Entity::getEulerAngles() const {
	return glm::degrees(glm::eulerAngles(_rotation));
}

void Entity::scale(glm::vec3 scale) {
	_modelMatrix = glm::scale(_modelMatrix, scale);
}

void Entity::rotate(glm::vec3 axis, float angle) {
	float length = glm::length(axis);
	if (1.0f > length + FLT_EPSILON ||
		1.0f < length - FLT_EPSILON)  // Normalize if needed
		axis = glm::normalize(axis);

	float angleInRad = glm::radians(angle);
	glm::quat rotation = glm::angleAxis(angleInRad, axis);
	_rotation = rotation * _rotation;
	_modelMatrix = glm::rotate(_modelMatrix, angleInRad, axis);
}

void Entity::rotateY(float angle) {
	float angleInRad = glm::radians(angle);
	glm::quat rotation = glm::angleAxis(angleInRad, glm::vec3(0.0, 1.0, 0.0));
	_rotation = rotation * _rotation;
	_modelMatrix =
		glm::rotate(_modelMatrix, angleInRad, glm::vec3(0.0, 1.0, 0.0));
}

void Entity::initEntity(GameEngine *gameEngine) {
	_gameEngine = gameEngine;
	_model = _gameEngine->getGameRenderer()->getModel(_modelName);
}

std::ostream &operator<<(std::ostream &o, Entity const &entity) {
	o << "---------- " << entity.getName() << " (Id: " << entity.getId() << ")"
	  << " ----------" << std::endl;

	glm::vec3 position = entity.getPosition();
	o << std::fixed << "Position => \033[1;33mx = " << position.x
	  << "\033[0m | \033[1;35my = " << position.y
	  << "\033[0m | \033[1;32mz = " << position.z << "\033[0m" << std::endl;

	glm::vec3 eulerAngles = entity.getEulerAngles();
	o << std::fixed << "Euler angles => \033[1;33mx = " << eulerAngles.x
	  << "\033[0m | \033[1;35my = " << eulerAngles.y
	  << "\033[0m | \033[1;32mz = " << eulerAngles.z << "\033[0m" << std::endl;

	glm::mat4 modelMatrix = entity.getModelMatrix();
	o << std::fixed << "Model Matrix = | " << modelMatrix[0][0] << " / "
	  << modelMatrix[1][0] << " / " << modelMatrix[2][0] << " / "
	  << modelMatrix[3][0] << " | " << std::endl;
	o << std::fixed << "               | " << modelMatrix[0][1] << " / "
	  << modelMatrix[1][1] << " / " << modelMatrix[2][1] << " / "
	  << modelMatrix[3][1] << " | " << std::endl;
	o << std::fixed << "               | " << modelMatrix[0][2] << " / "
	  << modelMatrix[1][2] << " / " << modelMatrix[2][2] << " / "
	  << modelMatrix[3][2] << " | " << std::endl;
	o << std::fixed << "               | " << modelMatrix[0][3] << " / "
	  << modelMatrix[1][3] << " / " << modelMatrix[2][3] << " / "
	  << modelMatrix[3][3] << " | " << std::endl;
	return o;
}
