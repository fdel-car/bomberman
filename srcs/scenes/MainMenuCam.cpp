#include "scenes/MainMenuCam.hpp"
#include "Player.hpp"
#include "GameEngine.hpp"

extern std::string _assetsDir;

MainMenuCam::MainMenuCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles) {
	vNeededImage.push_back(std::tuple<std::string, std::string>(
	(_assetsDir + "GUI/icons/chevronDroit.png"), "chevronDroit"));
	vNeededImage.push_back(std::tuple<std::string, std::string>(
	(_assetsDir + "GUI/icons/chevronGauche.png"), "chevronGauche"));
	vNeededImage.push_back(std::tuple<std::string, std::string>(
	(_assetsDir + "GUI/icons/settings.png"), "settings"));
}

MainMenuCam::~MainMenuCam(void) {}

void MainMenuCam::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(vNeededImage);

	graphicUI->uiSetDefaultFont("18_BOMBERMA");
	graphicUI->getDefaultStyle(THEME_DARK, &defaultStyle);
	defaultStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 0);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
}

void MainMenuCam::drawGUI(GUI * graphicUI) {

	int xPos = (WINDOW_W / 5);

	if (graphicUI->uiStartBlock("MainMenu0", "", nk_rect(0, (WINDOW_H / 5) * 1,WINDOW_W, 50), NK_WINDOW_NO_SCROLLBAR)) {
		graphicUI->uiHeader("MainTitle", NK_TEXT_CENTERED, 48, "48_BOMBERMA");
	}
	graphicUI->uiEndBlock();

	if (graphicUI->uiStartBlock("MainMenu1", "", nk_rect(xPos * 2, (WINDOW_H / 5) * 2, (WINDOW_W / 5), 50), NK_WINDOW_NO_SCROLLBAR)) {
		graphicUI->uiHorizontalSelection((WINDOW_W / 5), "Left2", "Right2");
	}
	graphicUI->uiEndBlock();

	if (graphicUI->uiStartBlock("MainMenu2", "", nk_rect(xPos  - (xPos / 2), (WINDOW_H / 5) * 4, (WINDOW_W / 5), 60), NK_WINDOW_NO_SCROLLBAR)) {
		if (graphicUI->uiHover()) {
		    if (graphicUI->uiButton((WINDOW_W / 5), 60, 0, "Settings", "settings", "14_BOMBERMA"))
		        std::cout << "3" << std::endl;
		}
		else {
			if (graphicUI->uiButton((WINDOW_W / 5), 60, NK_TEXT_RIGHT, "Settings", "", "14_BOMBERMA")) {
				std::cout << "3" << std::endl;
				_newSceneIdx = 1;
			}
		}
	}
	graphicUI->uiEndBlock();
	if (graphicUI->uiStartBlock("MainMenu3", "", nk_rect(xPos * 2, (WINDOW_H / 5) * 4, (WINDOW_W / 5), 60), NK_WINDOW_NO_SCROLLBAR)) {
	    if (graphicUI->uiButton((WINDOW_W / 5), 60, NK_TEXT_RIGHT, "", "settings", "14_BOMBERMA")) {
			std::cout << "2" << std::endl;
			_newSceneIdx = 1;
		}
	}
	graphicUI->uiEndBlock();

	if (graphicUI->uiStartBlock("MainMenu4", "", nk_rect(xPos * 4 - (xPos / 2), (WINDOW_H / 5) * 4, (WINDOW_W / 5), 60), NK_WINDOW_NO_SCROLLBAR)) {
	    if (graphicUI->uiButton((WINDOW_W / 5), 60,NK_TEXT_RIGHT, "Credit", "", "14_BOMBERMA")) {
			std::cout << "2" << std::endl;
		}
	}
	graphicUI->uiEndBlock();

}
