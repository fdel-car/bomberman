#include "engine/Light.hpp"

Light::Light(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Entity(pos, eulerAngles, nullptr, "", "Light", "Light") {
	_dir = _rotation * glm::vec3(0.0, 0.0, -1.0);
	_projection = glm::ortho<float>(0, WINDOW_W, 0, WINDOW_H, -100.0f, 100.0f);
}

Light::~Light(void) {}

glm::mat4 const &Light::getProjectionMatrix(void) const { return _projection; }
