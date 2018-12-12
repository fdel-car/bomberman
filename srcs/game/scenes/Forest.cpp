#include "game/scenes/Forest.hpp"
#include "engine/GameEngine.hpp"

Forest::Forest(WorldLocation &dialogueLocation, WorldLocation &gameplayLocation,
			   float transitionTime, Bomberman *bomberman)
	: SceneTools(17, 17, dialogueLocation, gameplayLocation, transitionTime,
				 bomberman, "Forest", "Pokemon"),
	  _cooldown(0.0f) {
	configAI();
	_startMusic = "Audio/Musics/Planet-Timbertree.wav";
	_initSoundsForGameplay();
}

Forest::~Forest(void) {}

void Forest::configAI(void) {
	_staticDecor.push_back("Wall");

	_tmpDecor.push_back("Explosion");
	_tmpDecor.push_back("OFDT");
	// _tmpDecor.push_back("EnemyRunAway");
	_tmpDecor.push_back("EPB");
	// _tmpDecor.push_back("EnemyBasic");
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
	if (true) {
		std::string str =
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
			"officia deserunt mollit anim id est laborum.";

		_buildNewDialogue(0, 0, 0, "Bomberman", "heart", str, false, 1000, 1000,
						  NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		_buildNewDialogue(0, 0, 0, "Bomberman", "heart", str, true, 1000, 1000,
						  NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
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
