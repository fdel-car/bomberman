#include "Factory.hpp"

Factory::Factory(void) {}

Factory::~Factory(void) {}

void Factory::load(void) {
	_camera = new Camera(glm::vec3(0.0, 10.0, -2.0), glm::vec3(0.0, 0.0, 0.0));

	t_data data;

	data.pos = glm::vec3(0.0, 0.0, 0.0);
	data.eulerAngles = glm::vec3(0.0, 0.0, 45.0f);  // Not considered for now
	data.collider = NULL;
	data.shapeName = "Floor";

	_vector.push_back(data);

	data.pos = glm::vec3(0.0, 1.0, 0.0);
	data.eulerAngles = glm::vec3(0.0, 0.0, 0.0);  // Not considered for now
	data.collider = NULL;
	data.shapeName = "Player";

	_vector.push_back(data);
}