#include "engine/Camera.hpp"
#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"

Camera::Camera(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Entity(pos, eulerAngles, nullptr, "", "Camera", "Camera"),
	  _newSceneIdx(-1),
	  _newSceneName("") {
	_view = glm::inverse(getModelMatrix());
	_speed = 8.0f;
	_front = -glm::normalize(glm::vec3(getModelMatrix()[2][0],
									   getModelMatrix()[2][1],
									   getModelMatrix()[2][2]));
	_right =
		glm::normalize(glm::vec3(getModelMatrix()[0][0], getModelMatrix()[0][1],
								 getModelMatrix()[0][2]));
	_up = glm::normalize(glm::cross(_right, _front));
}

int Camera::getNewSceneIdx(void) const { return _newSceneIdx; }

std::string Camera::getNewSceneName(void) const { return _newSceneName; }

bool Camera::isGameRunning(void) const { return _isRunning; }

bool Camera::isPause(void) const { return _debugMode || _isPause; }

std::vector<std::tuple<std::string, std::string>> const &
Camera::getNeededImages() const {
	return _neededImages;
}

void Camera::drawGUI(GUI *graphicUI) { (void)graphicUI; }

Camera::~Camera(void) {}

glm::mat4 const &Camera::getViewMatrix(void) const { return _view; };

glm::mat4 const &Camera::getProjectionMatrix(void) const { return _projection; }

void Camera::initEntity(GameEngine *gameEngine) {
	Entity::initEntity(gameEngine);
	_aspectRatio = (float)gameEngine->getGameRenderer()->getWidth() /
				   (float)gameEngine->getGameRenderer()->getHeight();
	_projection =
		glm::perspective(glm::radians(30.0f), _aspectRatio, 0.1f, 100.0f);
	_isRunning = true;
}

void Camera::configGUI(GUI *graphicUI) { (void)graphicUI; }

void Camera::update(void) {
	float deltaTime = _gameEngine->getDeltaTime();

	if (_gameEngine->isKeyJustPressed("`")) {
		_debugMode = !_debugMode;
		// Avoid camera jump on first frame
		_lastMousePos.x = _gameEngine->getGameRenderer()->getMousePos().x;
		_lastMousePos.y = _gameEngine->getGameRenderer()->getMousePos().y;
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
	if (_gameEngine->isKeyPressed(KEY_SPACE))
		translate(_up * deltaTime * _speed);
	if (_gameEngine->isKeyPressed(KEY_C)) translate(-_up * deltaTime * _speed);

	glm::vec2 mousePos = _gameEngine->getGameRenderer()->getMousePos();
	float xOffset = mousePos.x - _lastMousePos.x;
	float yOffset =
		_lastMousePos.y -
		mousePos.y;  // Reversed since y-coordinates range from bottom to top
	_lastMousePos.x = mousePos.x;
	_lastMousePos.y = mousePos.y;
	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	if (glm::epsilonNotEqual(0.0f, yOffset, EPSILON)) rotateX(yOffset);
	if (glm::epsilonNotEqual(0.0f, xOffset, EPSILON)) rotateY(-xOffset);
	_updateData();
}

void Camera::_updateData(void) {
	_right =
		glm::normalize(glm::vec3(getModelMatrix()[0][0], getModelMatrix()[0][1],
								 getModelMatrix()[0][2]));
	_front = -glm::normalize(glm::vec3(getModelMatrix()[2][0],
									   getModelMatrix()[2][1],
									   getModelMatrix()[2][2]));
	_up = glm::normalize(glm::cross(_right, _front));
	_view = glm::inverse(getModelMatrix());
}
