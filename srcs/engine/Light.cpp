#include "engine/Light.hpp"
#include "engine/GameEngine.hpp"

Light::Light(glm::vec2 const &lightPos, glm::vec3 const &lightTarget,
			 float gameDuration)
	: Entity(glm::vec3(lightPos.x, 0.0, lightPos.y), glm::vec3(0.0f), nullptr,
			 "", "Light", "Light"),
	  _target(lightTarget),
	  _color(glm::vec3(1.0f)) {
	_localOrientation = false;
	_projection = glm::ortho<float>(-40.0f, 40.0f, -40.0f, 40.0f, 1.0f, 100.0f);
	glm::vec3 tmp = (_target - getPosition());

	_rotationAxis = glm::cross(glm::vec3(0.0, 1.0, 0.0),
							   glm::normalize(glm::vec3(tmp.x, 0.0, tmp.z)));
	rotate(45.0f, _rotationAxis);
	_view = glm::lookAt(getPosition(), _target, glm::vec3(0.0, 1.0, 0.0));
	_dir = glm::normalize(_target - getPosition());

	_step = 90.0f / gameDuration;
	_total = 0.0f;
}

Light::~Light(void) {}

glm::vec3 const &Light::getDir(void) const { return _dir; }

glm::vec3 const &Light::getColor(void) const { return _color; }

glm::mat4 const &Light::getProjectionMatrix(void) const { return _projection; }

glm::mat4 const &Light::getViewMatrix(void) const { return _view; }

void Light::update(void) {
	float tmp = _step * _gameEngine->getDeltaTime();

	rotate(tmp, _rotationAxis);
	_total += tmp;
	if (_total >= 90.0f) {
		rotate(-90.0f, _rotationAxis);
		_total = 0.0f;
	}
	_view = glm::lookAt(getPosition(), _target, glm::vec3(0.0, 1.0, 0.0));
	_dir = glm::normalize(_target - getPosition());
}