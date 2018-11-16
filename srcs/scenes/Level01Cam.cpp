#include "scenes/Level01Cam.hpp"
#include "GameEngine.hpp"
#include "Player.hpp"

Level01Cam::Level01Cam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles) { }

Level01Cam::~Level01Cam(void) {}

void Level01Cam::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(vNeededImage);

	graphicUI->getDefaultStyle(THEME_DARK, &defaultStyle);
	defaultStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
}

void Level01Cam::drawGUI(GUI * graphicUI) {
	if (_gameEngine->isKeyPressed("I"))
		_newSceneIdx = 0;
	int xPos = (WINDOW_W / 5);
	if (graphicUI->uiStartBlock("MainMenu34", "", nk_rect(xPos * 2, (WINDOW_H / 5) * 4, (WINDOW_W / 5), 60), NK_WINDOW_NO_SCROLLBAR)) {
	    if (graphicUI->uiButton((WINDOW_W / 5), 60, NK_TEXT_RIGHT, "", "settings", "14_BOMBERMA")) {
			std::cout << "2" << std::endl;
			_newSceneIdx = 1;
		}
	}
	graphicUI->uiEndBlock();

	// graphicUI->uiDialogBox("Bomber Man", "image1.png", "NONONONONONONONO", false, 30, 1, NK_TEXT_CENTERED, "42_BOMBERMA.TTF", "18_BOMBERMA.TTF");
}
