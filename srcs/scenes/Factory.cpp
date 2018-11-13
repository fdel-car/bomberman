#include "Factory.hpp"

Factory::Factory(void) {}

Factory::~Factory(void) {}

void Factory::load(void) {
	t_data data;

	data.pos = glm::vec3(0.0, 0.0, 0.0);
	data.rot = glm::vec3(0.0, 0.0, 0.0);
	data.collider = NULL;
	data.modelName = "Bomb";

	_vector.push_back(data);

	data.pos = glm::vec3(0.0, 0.0, 0.0);
	data.rot = glm::vec3(0.0, 0.0, 0.0);
	data.collider = NULL;
	data.modelName = "Cube";

	_vector.push_back(data);
}