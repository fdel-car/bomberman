#include "engine/Camera.hpp"
#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"

Camera::Camera(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Entity(pos, eulerAngles, nullptr, "", "Camera", "Camera"),
	  _newSceneIdx(-1),
	  _newSceneName("") {
	_view = glm::inverse(getModelMatrix());
	_front = _rotation * glm::vec3(0.0, 0.0, -1.0);
	_speed = 6.0f;
}

int Camera::getNewSceneIdx(void) const { return _newSceneIdx; }

std::string Camera::getNewSceneName(void) const { return _newSceneName; }

std::vector<std::tuple<std::string, std::string>> const &
Camera::getNeededImages() const {
	return _neededImages;
}

void Camera::drawGUI(GUI *graphicUI) { (void)graphicUI; }

Camera::~Camera(void) {}

glm::mat4 const &Camera::getViewMatrix(void) const { return _view; };

glm::mat4 const &Camera::getProjectionMatrix(void) const { return _projection; }

// void Camera::updateProjectionMatrix()

void Camera::initEntity(GameEngine *gameEngine) {
	Entity::initEntity(gameEngine);
	_aspectRatio = (float)gameEngine->getGameRenderer()->getWidth() /
				   (float)gameEngine->getGameRenderer()->getHeight();
	_projection =
		glm::perspective(glm::radians(30.0f), _aspectRatio, 0.1f, 100.0f);
}

void Camera::configGUI(GUI *graphicUI) { (void)graphicUI; }

void Camera::update(void) {
	float deltaTime = _gameEngine->getDeltaTime();
	if (_gameEngine->isKeyPressed(KEY_UP)) {
		translate(_front * deltaTime * _speed);
		_updateData();
	}
	if (_gameEngine->isKeyPressed(KEY_DOWN)) {
		translate(-_front * deltaTime * _speed);
		_updateData();
	}
}

void Camera::_updateData(void) { _view = glm::inverse(getModelMatrix()); }
