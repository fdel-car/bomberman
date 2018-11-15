#include "Factory.hpp"
#include "Player.hpp"

Factory::Factory(void) {}

Factory::~Factory(void) {}

void Factory::load(void) {
	_camera = new Camera(glm::vec3(0.0, 10.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
	_entities.push_back(
		new Player(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 45.0f)));
	_entities.push_back(
		new Entity(glm::vec3(2.0, 0.0, 2.0), glm::vec3(0.0, 0.0, 45.0f),
				   new Collider(Collider::Rectangle, 1.0f, 1.0f), "Cube"));
}