#include "scenes/Level01Cam.hpp"
#include "GameEngine.hpp"
#include "Player.hpp"

Level01Cam::Level01Cam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles) { }

Level01Cam::~Level01Cam(void) {}

void Level01Cam::drawGUI(GUI * graphicUI) {
	if (_gameEngine->isKeyPressed("I"))
		_newSceneIdx = 0;

	graphicUI->uiDialogBox("Bomber Man", "image1.png", "NONONONONONONONO", false, 30, 1, NK_TEXT_CENTERED, "42_BOMBERMA.TTF", "18_BOMBERMA.TTF");
}
