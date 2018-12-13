#include "game/scenes/Pokemon.hpp"
#include "engine/GameEngine.hpp"

Pokemon::Pokemon(WorldLocation &dialogueLocation,
				 WorldLocation &gameplayLocation, float transitionTime,
				 Bomberman *bomberman)
	: SceneTools(17, 25, dialogueLocation, gameplayLocation, transitionTime,
				 bomberman, "Pokemon", "Mario"),
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
	if (true) {
		std::string str = "Oh no...";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Nice, I love pokemon! :D But I prefer Kyogre.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"I know, I'm you, remember? But let's not complain, now "
			" you have to neutralize the digglet.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Why? The portal isn't in a fossil?";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"It was, but I've seen this digglet stole it and now you'll have "
			"to kill him to survive. He's a fast runner for sure.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"Don't worry I'm gonna blow away this little thief, he should not "
			"have messed with me!";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Do not count your chickens before they hatch. Go get him now!";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
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
