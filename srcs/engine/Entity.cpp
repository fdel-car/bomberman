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
	_eulerAngles = eulerAngles;
	_rotation = glm::quat(glm::radians(eulerAngles));
	_scale = glm::vec3(1.0f);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
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

void Entity::onTriggerEnter(Entity *entity) { (void)entity; }

GameEngine *Entity::getGameEngine(void) const { return _gameEngine; }

const glm::vec3 &Entity::getPosition(void) const { return _position; }

const glm::mat4 &Entity::getModelMatrix(void) const { return _modelMatrix; }

const Collider *Entity::getCollider(void) const { return _collider; }

const Model *Entity::getModel(void) const { return _model; }

bool Entity::getTmpState(void) const { return _isTmp; }

size_t const &Entity::getId(void) const { return _id; }

std::string const &Entity::getName(void) const { return _name; }

std::string const &Entity::getTag(void) const { return _tag; }

glm::vec3 const &Entity::getTargetMovement(void) const {
	return _targetMovement;
}

bool Entity::getNeedToBeDestroyed(void) const { return _needToBeDestroyed; }

void Entity::translate(glm::vec3 translation) {
	_position += translation;
	_updateData();

	// Signal, if _sceneManager is set, what is the entity new position
	if (_sceneManager != nullptr) _sceneManager->tellPosition(this);
}

glm::vec3 const &Entity::getEulerAngles() const { return _eulerAngles; }

void Entity::scale(glm::vec3 scale) {
	_scale *= scale;
	_updateData();
}

void Entity::rotateX(float angle) {
	_eulerAngles.x += angle;
	_updateData();
}

void Entity::rotateY(float angle) {
	_eulerAngles.y += angle;
	_updateData();
}

void Entity::_updateData(void) {
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	translationMatrix[3][0] += _position.x;
	translationMatrix[3][1] += _position.y;
	translationMatrix[3][2] += _position.z;
	_rotation = glm::quat(glm::radians(_eulerAngles));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
	_modelMatrix = translationMatrix * glm::mat4_cast(_rotation) * scaleMatrix;
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
