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
	if (true) {
		std::string str = "Wow, we are in space.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "You took a lot of LSD at this party.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "And i can fly !";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "It's a dream, you are still dying in the real life...";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Oh yes i forgot this. What am i supposed to do this time for survive ?";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "You have to find the last portal for comeback in the real life. But i don't know where is the portal this time... I'm sorry.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Don't worry i'm will blow everything for find it.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Just care about the red ghosts. They will focus you if you give them a way to you.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "YYYYYY AAAAAA AAAAAA AAAAAA HHHHHH HHHHHH.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1,
				  			NK_TEXT_CENTERED, "20_BOMBERMAN", "24_BOMBERMAN");
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
