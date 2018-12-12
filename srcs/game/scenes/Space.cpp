#include "game/scenes/Space.hpp"
#include "engine/GameEngine.hpp"

Space::Space(WorldLocation &dialogueLocation, WorldLocation &gameplayLocation,
			 float transitionTime,
			 Bomberman *bomberman)
	:  // Camera(pos, eulerAngles),
	  SceneTools(29, 21, dialogueLocation, gameplayLocation, transitionTime,
				 bomberman, "Space", "Credits"),
	  _cooldown(0.0f) {
	configAI();
	_initSoundsForGameplay();
	_startMusic = "Audio/Musics/Mystery.wav";
}

Space::~Space(void) {}

void Space::configAI(void) {
	_staticDecor.push_back("Wall");

	_tmpDecor.push_back("Explosion");
	_tmpDecor.push_back("EnemyMelee");
	_tmpDecor.push_back("EnemyBasic");
	_tmpDecor.push_back("Bomb");
	_tmpDecor.push_back("Box");
}

void Space::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImages(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_refreshAI = false;
	if (false) {
		std::string str =
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
			"officia deserunt mollit anim id est laborum.";

		_buildNewDialogue(0, 0, 0, "Bomberman", "heart", str, false, 1000, 1000,
						  NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		_buildNewDialogue(0, 0, 0, "Bomberman", "heart", str, true, 1000, 1000,
						  NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
	}
}

void Space::tellPosition(Entity *entity) { _savePositions(entity); }

void Space::update(void) {
	Camera::update();
	_refreshAI = false;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.2f;
		_startBuildingGrapheForPathFinding();
		_refreshAI = true;
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
