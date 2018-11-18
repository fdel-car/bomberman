#include "scenes/Level01Cam.hpp"
#include "GameEngine.hpp"
#include "Player.hpp"

Level01Cam::Level01Cam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles) { }

Level01Cam::~Level01Cam(void) {}

void Level01Cam::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(vNeededImage);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_pauseMenu = false;
}

void Level01Cam::drawGUI(GUI * graphicUI) {
	if (_pauseMenu || _gameEngine->isKeyPressed("E")) {
		_pauseMenu = true;
		if (graphicUI->uiStartBlock(
			"PauseMenu", "Pause",
			nk_rect((WINDOW_W / 2) - (WINDOW_W / 8),
			(WINDOW_H / 3), WINDOW_W / 4, WINDOW_H / 3),
			NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
			if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
				"Resume", "", "14_BOMBERMA")) {
				_pauseMenu = false;
			}
			if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
				"Restart level", "", "14_BOMBERMA")) {
				_newSceneIdx = 1;
			}
			if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
				"Quit level", "", "14_BOMBERMA")) {
				_newSceneIdx = 0;
			}
			if (graphicUI->uiButton(0, 0, 0, "", "", "")) { }
		}
		graphicUI->uiEndBlock();
	}
	// graphicUI->uiDialogBox("Bomber Man", "image1.png", "NONONONONONONONO", false, 30, 1, NK_TEXT_CENTERED, "42_BOMBERMA.TTF", "18_BOMBERMA.TTF");
}
