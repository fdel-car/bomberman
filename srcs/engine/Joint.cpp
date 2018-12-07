#include "engine/Joint.hpp"

Joint::Joint(std::string const &name, glm::mat4 const &offsetMatrix, int index)
	: name(name), offsetMatrix(offsetMatrix), index(index) {}

Joint::~Joint(void) {}

void Joint::updateFinalTransform(void) {
	Joint *tmp = parent;
	std::vector<glm::mat4> transforms;

	finalTransform = _toYAxisUp;
	while (tmp != nullptr) {
		transforms.push_back(tmp->localTransform);
		tmp = tmp->parent;
	}
	for (int i = transforms.size() - 1; i >= 0; i--)
		finalTransform *= transforms[i];
	finalTransform *= localTransform * offsetMatrix;
}

glm::mat4 Joint::_toYAxisUp = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
										  glm::vec3(1.0, 0.0, 0.0));