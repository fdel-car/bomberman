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
		std::string str = "Wow, my head is spinning...";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "What happened? Where am I?";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"Hello there, don't remember yesterday's party?  You tried LSD for "
			"the first time. And right now you are stuck in a dream, dying "
			"from overdose.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "WHAT?! Am I dead?! Who is talking?";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"I'm your subconscious... And no you're not, at least not yet. I'm "
			"here to help you. If you want to survive you have to listen to "
			"me. First, let's find the portal in the wooden boxes. But care "
			"about the Fuzzies, they can hurt you badly.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"Oh, but how am I supposed to destroy those wooden boxes? And "
			"what's a Fuzzy?";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str =
			"Hey, use your brain and your bombs. And a Fuzzy is a kind of "
			"little hairball, there are many of them in here apparently. "
			"Just don't touch them. Hurry up, we don't have much time!";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000,
						  1000, NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
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
