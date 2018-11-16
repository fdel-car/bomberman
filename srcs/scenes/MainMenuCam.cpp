#include "scenes/MainMenuCam.hpp"
#include "Player.hpp"
#include "GameEngine.hpp"

MainMenuCam::MainMenuCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles) { }

MainMenuCam::~MainMenuCam(void) {}

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
		graphicUI->uiWidget(50);
		if (graphicUI->uiHover()) {
		    if (graphicUI->uiButton(0, "Settings", "settings", "14_BOMBERMA"))
		        std::cout << "3" << std::endl;
		}
		else {
			if (graphicUI->uiButton(NK_TEXT_RIGHT, "Settings", "", "14_BOMBERMA")) {
				std::cout << "3" << std::endl;
				_newSceneIdx = 1;
			}
		}
	}
	graphicUI->uiEndBlock();

	if (graphicUI->uiStartBlock("MainMenu3", "", nk_rect(xPos * 2, (WINDOW_H / 5) * 4, (WINDOW_W / 5), 60), NK_WINDOW_NO_SCROLLBAR)) {
		graphicUI->uiWidget(50);
	    if (graphicUI->uiButton(NK_TEXT_RIGHT, "Level 1", "", "14_BOMBERMA")) {
			std::cout << "2" << std::endl;
			_newSceneIdx = 1;
		}
	}
	graphicUI->uiEndBlock();

	if (graphicUI->uiStartBlock("MainMenu4", "", nk_rect(xPos * 4 - (xPos / 2), (WINDOW_H / 5) * 4, (WINDOW_W / 5), 60), NK_WINDOW_NO_SCROLLBAR)) {
		graphicUI->uiWidget(50);
	    if (graphicUI->uiButton(NK_TEXT_RIGHT, "Credit", "", "14_BOMBERMA")) {
			std::cout << "2" << std::endl;
		}
	}
	graphicUI->uiEndBlock();

	// if (_gameEngine->isKeyPressed("I"))
	// 	_newSceneIdx = 1;
	// if (_gameEngine->isKeyPressed("Q"))
	// 	graphicUI->uiDialogBox("Bomber Man", "image1.png", "AHAHAHAHAHAH AH", false, 40, 1, NK_TEXT_CENTERED, "42_BOMBERMA.TTF", "18_BOMBERMA.TTF");
}
