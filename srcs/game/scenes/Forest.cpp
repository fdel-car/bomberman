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
		std::string str = "Wow, my head is hurting...";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "What happened ? Where am i ?";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "You was in a party and you tried LSD for the first time. And now you are in a dream, dying from an overdose.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Who is talking ? WHAT i'm DEAD ????";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "I'm your subconscious... No you're not. I'm here for helping you. If you want to survive you have to listen me. You have to find a portal in the wooden boxes. But care about the Fuzzy, they hurt.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "Ok ok but how am i supposed to destroy the wooden boxes ? And what is a Fuzzy ?";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberWhite", str, false, 1000, 1000,
				  			NK_TEXT_LEFT, "20_BOMBERMAN", "24_BOMBERMAN");
		str = "For the boxes use your brain and your bombs... And for the Fuzzy they are little hairballs, just don't touch them. You should hurry. We don't have time to talk.";
		_buildNewDialogue(0, 0, 0, "Bomberman", "BomberBlack", str, true, 1000, 1000,
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
