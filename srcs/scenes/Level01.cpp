#include "scenes/Level01.hpp"
#include "scenes/Level01Cam.hpp"
#include "Player.hpp"

Level01::Level01(void) {}

Level01::~Level01(void) {}

void Level01::load(void) {
	_camera = new Level01Cam(glm::vec3(0.0, 10.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

	_entities.push_back(
		new Player(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 45.0f)));
	_entities.push_back(
		new Entity(glm::vec3(2.0, 0.0, 2.0), glm::vec3(0.0, 0.0, 45.0f),
				   new Collider(Collider::Rectangle, 1.0f, 1.0f), "Cube"));
}
