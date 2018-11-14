#include "Factory.hpp"

Factory::Factory(void) {}

Factory::~Factory(void) {}

void Factory::load(void) {
	t_data data;

	data.pos = glm::vec3(1.0, 0.0, 0.0);
	data.eulerAngles = glm::vec3(0.0, 45.0, 0.0);
	data.collider = NULL;
	data.shapeName = "Cube";

	_vector.push_back(data);

	// data.pos = glm::vec3(0.0, 0.0, 0.0);
	// data.eulerAngles = glm::vec3(0.0, 0.0, 0.0);
	// data.collider = NULL;
	// data.shapeName = "Sponza";

	// _vector.push_back(data);
}