#include "engine/Camera.hpp"
#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"

Camera::Camera(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Entity(pos, eulerAngles, nullptr, ""),
	  _newSceneIdx(-1),
	  _newSceneName("") {
	_name = "Camera";
	_view = glm::inverse(getModelMatrix());
	_speed = 8.0f;
	_front = _rotation * glm::vec3(0.0, 0.0, -1.0);
	_right =
		glm::normalize(glm::vec3(getModelMatrix()[0][0], getModelMatrix()[0][1],
								 getModelMatrix()[0][2]));
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

	if (_gameEngine->isKeyJustPressed("`")) {
		_debugMode = !_debugMode;
		_gameEngine->getGameRenderer()->switchCursorMode(_debugMode);
	}
	if (!_debugMode) return;

	if (_gameEngine->isKeyPressed(KEY_W))
		translate(_front * deltaTime * _speed);
	if (_gameEngine->isKeyPressed(KEY_D))
		translate(_right * deltaTime * _speed);
	if (_gameEngine->isKeyPressed(KEY_S))
		translate(-_front * deltaTime * _speed);
	if (_gameEngine->isKeyPressed(KEY_A))
		translate(-_right * deltaTime * _speed);
	_updateData();
}

void Camera::_updateData(void) { _view = glm::inverse(getModelMatrix()); }
