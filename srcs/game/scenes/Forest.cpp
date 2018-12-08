#include "game/scenes/Forest.hpp"
#include "engine/GameEngine.hpp"

extern std::string _assetsDir;

Forest::Forest(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
			   Bomberman *bomberman)
	:  // Camera(pos, eulerAngles),
	  SceneTools(21, 21, pos, eulerAngles, bomberman, "Forest", "Volcano"),
	  _cooldown(0.0f) {
	configAI();
	_startMusic = _assetsDir + "Audio/Musics/Planet-Timbertree.wav";
	_initSoundsForGameplay();
}

Forest::~Forest(void) {}

void Forest::configAI(void) {
	_staticDecor.push_back("Wall");

	_tmpDecor.push_back("Explosion");
	_tmpDecor.push_back("OFDT");
	_tmpDecor.push_back("RunAway");
	_tmpDecor.push_back("EPB");
	_tmpDecor.push_back("Basic");
	_tmpDecor.push_back("Bomb");
	_tmpDecor.push_back("Box");

	_tmpDecorForRunAway.push_back("Player");
	_tmpDecorForRunAway.push_back("Explosion");
	_tmpDecorForRunAway.push_back("Bomb");
	_tmpDecorForRunAway.push_back("Box");
}

void Forest::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImages(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_refreshAI = false;
	if (false) {
		std::string str =
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
			"officia deserunt mollit anim id est laborum.";

		_dialogues.push_back(_builNewDialogue(0, 0, 0, "Bomberman", "heart", str, false, 1000, 1000, NK_TEXT_LEFT,
						 "20_BOMBERMAN", "24_BOMBERMAN"));
		_dialogues.push_back(_builNewDialogue(0, 0, 0, "Bomberman", "heart", str, true, 1000, 1000, NK_TEXT_LEFT,
						 "20_BOMBERMAN", "24_BOMBERMAN"));
	}
}

void Forest::drawGUI(GUI *graphicUI) {
	if (_dialogues.empty())
		SceneTools::drawGUI(graphicUI);
	else {
		_isPause = true;
		if (!_displayMultipleDialogue(graphicUI, &_dialogues))
			_isPause = false;
	}
}

void Forest::tellPosition(Entity *entity) { _savePositions(entity); }

void Forest::update(void) {
	Camera::update();

	_refreshAI = false;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.1f;
		_startBuildingGrapheForPathFinding();
		_refreshAI = true;
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
