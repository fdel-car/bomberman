#include "game/scenes/Mario.hpp"
#include "engine/GameEngine.hpp"

Mario::Mario(WorldLocation &dialogueLocation, WorldLocation &gameplayLocation,
			 float transitionTime, Bomberman *bomberman)
	: SceneTools(41, 13, dialogueLocation, gameplayLocation, transitionTime,
				 bomberman, "Mario", "Space"),
	  _cooldown(0.0f) {
	configAI();
	_initSoundsForGameplay();
	_startMusic = "Audio/Musics/Robots.wav";  // TODO
}

Mario::~Mario(void) {}

void Mario::configAI(void) {
	_staticDecor.push_back("Wall");

	_tmpDecor.push_back("Explosion");
	_tmpDecor.push_back("EnemyBasic");
	_tmpDecor.push_back("EnemyBomber");
	_tmpDecor.push_back("Bomb");
	_tmpDecor.push_back("Box");

	_tmpDecorForRunAway.push_back("Explosion");
	_tmpDecorForRunAway.push_back("Bomb");
	_tmpDecorForRunAway.push_back("Box");
}

void Mario::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImages(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_refreshAI = false;
	if (true) {
		std::string str =
			"Well, this time the portal is probably in a rock pipe. You should "
			"be able to destroy them with your bombs.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "The koffings are so cutes!";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"Can you listen please? They are not cute, they are dangerous. "
			"They can blow you up if you stay static to long.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Roger, the koffings are not cutes.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
	}
}

void Mario::tellPosition(Entity *entity) { _savePositions(entity); }

void Mario::update(void) {
	Camera::update();
	_refreshAI = false;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.1f;
		_startBuildingGrapheForPathFinding();
		_refreshAI = true;
		// printMapInfo();
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
