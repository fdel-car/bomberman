#include "game/scenes/Load.hpp"

Load::Load(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
		   Bomberman *bomberman)
	: SceneTools(0, 0, pos, eulerAngles, bomberman,
				 bomberman->getStartLevelName()) {}

Load::~Load(void) {}

void Load::drawGUI(GUI *graphicUI) {
	if (graphicUI->uiStartBlock(
			"loadingText", "",
			nk_rect((WINDOW_W / 5) * 2, (WINDOW_H / 5) * 2, (WINDOW_W / 5), 50),
			NK_WINDOW_NO_SCROLLBAR)) {
		activeStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 0);
		activeStyle[NK_COLOR_TEXT] = nk_rgba(255, 255, 255, 255);
		graphicUI->setStyle(activeStyle);
		activeStyle = defaultStyle;
		static int nbrOfPoint = 0;
		static bool addPoint = true;
		static float timer = 0;

		std::string text = "Loading";
		if (timer <= 0.0f) {
			if (addPoint && nbrOfPoint < 3)
				nbrOfPoint++;
			else if (!addPoint && nbrOfPoint > 0)
				nbrOfPoint--;
			else
				addPoint = !addPoint;
			timer = 0.08f;
		}
		timer -= _gameEngine->getDeltaTime();
		for (int i = 0; i < nbrOfPoint; i++) {
			text += " .";
		}
		graphicUI->uiHeader(text.c_str(), NK_TEXT_LEFT, 48, "35_BOMBERMAN");
		graphicUI->setStyle(activeStyle);
	}
	graphicUI->uiEndBlock();
}
