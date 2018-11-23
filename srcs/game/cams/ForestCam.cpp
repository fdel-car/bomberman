#include "game/cams/ForestCam.hpp"
#include "engine/GameEngine.hpp"

ForestCam::ForestCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	:  // Camera(pos, eulerAngles),
	  Tools(17, 17, pos, eulerAngles) {}

ForestCam::~ForestCam(void) {}

void ForestCam::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_pauseMenu = false;
}

void ForestCam::drawGUI(GUI *graphicUI) {
	if (_pauseMenu || _gameEngine->isKeyPressed("E"))
		_pauseMenu = _displayPauseMenu(graphicUI, &_newSceneIdx, 1, 0);

	// static int searchWord = 0;
	// static int lastWord = 0;
	// static int startStrIdx = 0;
	// std::string str =
	// 	"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
	// 	"officia deserunt mollit anim id est laborum.";
	// _displayDialogue(graphicUI, &searchWord, &lastWord, &startStrIdx,
	// 				 "Bomberman", "", str, false, 1000, 1000,
	// 				 NK_TEXT_LEFT, "12_BOMBERMAN", "18_BOMBERMAN");
}
