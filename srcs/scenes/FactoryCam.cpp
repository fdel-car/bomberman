#include "scenes/FactoryCam.hpp"
#include "GameEngine.hpp"
#include "Player.hpp"

FactoryCam::FactoryCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles) {}

FactoryCam::~FactoryCam(void) {}

void FactoryCam::drawGUI(GUI *graphicUI) {
	if (_gameEngine->isKeyPressed("I")) _newSceneIdx = 0;

	graphicUI->uiDialogBox("Bomberman", "image1.png", "", false, 30, 1,
						   NK_TEXT_CENTERED, "42_BOMBERMAN.ttf",
						   "18_BOMBERMAN.ttf");
}
