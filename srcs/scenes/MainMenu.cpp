#include "scenes/MainMenu.hpp"
#include "scenes/MainMenuCam.hpp"
#include "Player.hpp"

MainMenu::MainMenu(void) {}

MainMenu::~MainMenu(void) {}

void MainMenu::load(void) {
	_camera = new MainMenuCam(glm::vec3(0.0, 10.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

	_entities.push_back(
		new Player(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 45.0f)));
}
