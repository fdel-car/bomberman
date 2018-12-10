#include "game/scenes/Pokemon.hpp"
#include "engine/GameEngine.hpp"

Pokemon::Pokemon(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
				 Bomberman *bomberman)
	: SceneTools(17, 25, pos, eulerAngles, bomberman, "Pokemon", "Mario"),
	  _cooldown(0.0f) {
	configAI();
	_startMusic = "Audio/Musics/Town.wav";
	_initSoundsForGameplay();
}

Pokemon::~Pokemon(void) {}

void Pokemon::configAI(void) {
	_staticDecor.push_back("Wall");

	_tmpDecor.push_back("Explosion");
	_tmpDecor.push_back("OFDT");
	_tmpDecor.push_back("EPB");
	_tmpDecor.push_back("Bomb");
	_tmpDecor.push_back("Box");

	_tmpDecorForRunAway.push_back("Player");
	_tmpDecorForRunAway.push_back("Explosion");
	_tmpDecorForRunAway.push_back("Bomb");
	_tmpDecorForRunAway.push_back("Box");
}

void Pokemon::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImages(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_refreshAI = false;
	if (false) {
		std::string str =
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
			"officia deserunt mollit anim id est laborum.";

		_dialogues.push_back(_builNewDialogue(
			0, 0, 0, "Bomberman", "heart", str, false, 1000, 1000, NK_TEXT_LEFT,
			"20_BOMBERMAN", "24_BOMBERMAN"));
		_dialogues.push_back(_builNewDialogue(
			0, 0, 0, "Bomberman", "heart", str, true, 1000, 1000, NK_TEXT_LEFT,
			"20_BOMBERMAN", "24_BOMBERMAN"));
	}
}

void Pokemon::drawGUI(GUI *graphicUI) {
	if (_dialogues.empty())
		SceneTools::drawGUI(graphicUI);
	else {
		_isPause = true;
		if (!_displayMultipleDialogue(graphicUI, &_dialogues)) _isPause = false;
	}
}

void Pokemon::tellPosition(Entity *entity) { _savePositions(entity); }

void Pokemon::update(void) {
	Camera::update();

	_refreshAI = false;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.1f;
		_startBuildingGrapheForPathFinding();
		_refreshAI = true;
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
