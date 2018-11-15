#include "Camera.hpp"

Camera::Camera(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Entity(pos, eulerAngles, nullptr, "") {
	// All of that initialisation is temporary
	// glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	// glm::vec3 cameraInvDir = glm::normalize(getPosition() - cameraTarget);
	// glm::vec3 up = glm::vec3(0.0f, 0.0f, -1.0f);
	// glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraInvDir));
	// glm::vec3 cameraUp = glm::cross(cameraInvDir, cameraRight);
	_view = glm::lookAt(getPosition(), glm::vec3(0.0f, -1.0f, 0.0f),
						glm::vec3(0.0f, 0.0f, -1.0f));
}

Camera::~Camera(void) {}

glm::mat4 const &Camera::getViewMatrix(void) const { return _view; };