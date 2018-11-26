#include "engine/Light.hpp"

Light::Light(glm::vec3 const &pos, glm::vec3 const &lightTarget)
	: Entity(pos, glm::vec3(0.0f), nullptr, "", "Light", "Light"),
	  _color(glm::vec3(1.0f)) {  // TODO: give correct euler angles and update
								 // them when needed
	_projection = glm::ortho<float>(
		-40.0f, 40.0f, -40.0f, 40.0f, 0.1f,
		100.0f);  // TODO: Replace raw values with dynamic ones if needed
	_view = glm::lookAt(getPosition(), lightTarget, glm::vec3(0.0, 1.0, 0.0));
	_dir = glm::normalize(lightTarget - getPosition());
}

Light::~Light(void) {}

glm::vec3 const &Light::getDir(void) const { return _dir; }

glm::vec3 const &Light::getColor(void) const { return _color; }

glm::mat4 const &Light::getProjectionMatrix(void) const { return _projection; }

glm::mat4 const &Light::getViewMatrix(void) const { return _view; }

void Light::update(void) {}