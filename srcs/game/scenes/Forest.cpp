#include "engine/GameEngine.hpp"
#include "game/scenes/Forest.hpp"

Forest::Forest(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	:  // Camera(pos, eulerAngles),
	  SceneTools(17, 17, pos, eulerAngles), _cooldown(0.5f) {
	_light = new Entity(glm::vec3(-10.0f, 10.0f, 10.0f),
						glm::vec3(45.0f, 45.0f, 45.0f), nullptr, "Box", "Light",
						"Light");
	configAI();
}

Forest::~Forest(void) {}

void Forest::configAI(void) {
	_staticDecor.push_back("Wall");
	// _staticDecor.push_back("Explosion");

	// _tmpDecor.push_back("Bomb");
}

void Forest::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_refreshAI = false;
	_pauseMenu = false;
}

void Forest::drawGUI(GUI *graphicUI) {
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

void Forest::tellPosition(Entity *entity) { _savePositions(entity); }

void Forest::update(void) {
	Camera::update();
	_refreshAI = false;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.5f;
		_startBuildingGrapheForPathFinding();
		_refreshAI = true;
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
