#include "engine/Entity.hpp"
#include "engine/Camera.hpp"
#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include "glm/gtx/euler_angles.hpp"

size_t Entity::_spawnedEntities = 0;

void Entity::resetSpawnedEntities(void) { _spawnedEntities = 0; }

Entity::Entity(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
			   std::string modelName, std::string name, std::string tag,
			   Entity *sceneManager)
	: _position(position),
	  _id(Entity::_spawnedEntities++),
	  _modelName(modelName),
	  _name(name),
	  _tag(tag),
	  _sceneManager(sceneManager),
	  _collider(collider),
	  _targetMovement(glm::vec3()),
	  _neededSounds(std::set<std::string>()),
	  _initSounds(std::vector<std::string>()),
	  _destroySounds(std::vector<std::string>()) {
	_translationMatrix[3][0] = position.x;
	_translationMatrix[3][1] = position.y;
	_translationMatrix[3][2] = position.z;
	_rotationMatrix = glm::mat4(glm::quat(glm::radians(eulerAngles)));
	_eulerAngles = eulerAngles;
	_updateModelMatrix();

	// Signal, if _sceneManager is set, where is entity starting pos
	if (_sceneManager != nullptr) _sceneManager->tellPosition(this);
}

Entity::~Entity(void) {
	// Signal, if _sceneManager is set, that entity will be destroyed
	if (_sceneManager != nullptr) {
		_sceneManager->tellDestruction(this);
	}
	if (_collider) delete _collider;
	if (_destroySounds.size() != 0) {
		_gameEngine->playSound(_destroySounds[rand() % _destroySounds.size()]);
	}
}

void Entity::update(void) {}

void Entity::tellPosition(Entity *entity) { (void)entity; }

void Entity::tellDestruction(Entity *entity) { (void)entity; }

void Entity::onCollisionEnter(Entity *entity) { (void)entity; }

void Entity::onTriggerEnter(Entity *entity) { (void)entity; }

GameEngine *Entity::getGameEngine(void) const { return _gameEngine; }

const glm::vec3 &Entity::getPosition(void) const { return _position; }

const glm::vec3 &Entity::getColor(void) const { return _color; }

const glm::mat4 &Entity::getModelMatrix(void) const { return _modelMatrix; }

const Collider *Entity::getCollider(void) const { return _collider; }

Model *Entity::getModel(void) const { return _model; }

void Entity::updateModel(void) {
	_model = _gameEngine->getGameRenderer()->getModel(_modelName);
}

size_t const &Entity::getId(void) const { return _id; }

glm::vec3 const &Entity::getEulerAngles() const {
	return _eulerAngles;  // Could be false since rotate does not update them
}

std::string const &Entity::getName(void) const { return _name; }

std::string const &Entity::getTag(void) const { return _tag; }

std::string const &Entity::getModelName(void) const { return _modelName; }

glm::vec3 const &Entity::getTargetMovement(void) const {
	return _targetMovement;
}

bool Entity::needsToBeDestroyed(void) const { return _needToBeDestroyed; }

std::set<std::string> Entity::getNeededSounds(void) const {
	return _neededSounds;
};

bool Entity::doShowModel(void) const { return _showModel; }

void Entity::setColor(glm::vec3 const &color) { _color = color; }

void Entity::resetColor(void) { _color = glm::vec3(-1.0f); }

void Entity::translate(glm::vec3 translation) {
	_translationMatrix = glm::translate(_translationMatrix, translation);
	_updateModelMatrix();

	// Signal, if _sceneManager is set, what is the entity new position
	if (_sceneManager != nullptr) _sceneManager->tellPosition(this);
}

void Entity::scale(glm::vec3 scale) {
	_scaleMatrix = glm::scale(_scaleMatrix, scale);
	_updateModelMatrix();
}

void Entity::rotate(float angle, glm::vec3 axis) {
	_rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(angle), axis);
	// TODO: Update euler angles correctly
	_updateModelMatrix();
}

void Entity::rotateX(float angle) {
	_rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(angle),
								  glm::vec3(1.0, 0.0, 0.0));
	_eulerAngles.x = fmod(_eulerAngles.x + angle, 360.0f);
	_updateModelMatrix();
}

void Entity::rotateY(float angle) {
	glm::mat4 tmp = glm::rotate(glm::mat4(1.0f), glm::radians(angle),
								glm::vec3(0.0, 1.0, 0.0));
	_rotationMatrix = tmp * _rotationMatrix;
	_eulerAngles.y = fmod(_eulerAngles.y + angle, 360.0f);
	_updateModelMatrix();
}

void Entity::_updateModelMatrix(void) {
	if (_localOrientation)
		_modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
	else
		_modelMatrix = _rotationMatrix * _translationMatrix * _scaleMatrix;
	_position.x = _modelMatrix[3][0];
	_position.y = _modelMatrix[3][1];
	_position.z = _modelMatrix[3][2];
}

void Entity::initEntity(GameEngine *gameEngine) {
	_gameEngine = gameEngine;
	updateModel();
	if (_initSounds.size() != 0) {
		gameEngine->playSound(_initSounds[rand() % _initSounds.size()]);
	}
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
