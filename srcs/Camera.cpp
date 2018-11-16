#include "Camera.hpp"
#include "GameEngine.hpp"
#include "GameRenderer.hpp"

Camera::Camera(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Entity(pos, eulerAngles, nullptr, ""), _newSceneIdx(-1) {
	_name = "Camera";
	_view = glm::inverse(getModelMatrix());
	_front = _rotation * glm::vec3(0.0, 0.0, -1.0);
	_speed = 6.0f;
}

int Camera::getNewSceneIdx(void) { return _newSceneIdx; }

void Camera::drawGUI(GUI *graphicUI) { (void)graphicUI; }

Camera::~Camera(void) {}

glm::mat4 const &Camera::getViewMatrix(void) const { return _view; };

glm::mat4 const &Camera::getProjectionMatrix(void) const { return _projection; }

std::vector<std::tuple<std::string, std::string>>
	&Camera::getNeededImage() {
	return vNeededImage;
}

// void Camera::updateProjectionMatrix()

void Camera::initEntity(GameEngine *gameEngine) {
	Entity::initEntity(gameEngine);
	_aspectRatio = (float)gameEngine->getGameRenderer()->getWidth() /
				   (float)gameEngine->getGameRenderer()->getHeight();
	// _projection =
	// glm::perspective(glm::radians(45.0f), _aspectRatio, 0.1f, 100.0f);
	float length = glm::length(glm::vec3() - getPosition());
	_projection = glm::ortho(-_aspectRatio * length, _aspectRatio * length,
							 -length, length, 0.1f, 100.0f);
}

void Camera::configGUI(GUI *graphicUI) {
	(void)graphicUI;
}


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

void Camera::_updateData(void) {
	_view = glm::inverse(getModelMatrix());
	float length = glm::length(glm::vec3() - getPosition());
	_projection = glm::ortho(-_aspectRatio * length, _aspectRatio * length,
							 -length, length, 0.1f, 100.0f);
}
