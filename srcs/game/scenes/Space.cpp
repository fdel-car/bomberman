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
		std::string str = "We are in space!";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "You did take a lot of LSD at this party.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Worth it, I can fly now!";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "It's a dream, and you're still dying in real life...";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"Oh yes I totally forgot about this. What am I supposed to do this "
			"time to survive?";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"You have to find the last portal to escape and get back to your "
			"real body. But I don't know where is it this time... I'm sorry.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "So I'll blow everything up to find it.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"Just be careful about the ghosts. They will chase you if they can "
			"find a path.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Let's GO! YEAAAHHH!";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1, NK_TEXT_CENTERED, "20_BOMBERMAN", "24_BOMBERMAN");
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
