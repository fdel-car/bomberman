#include "game/scenes/Desert.hpp"
#include "engine/GameEngine.hpp"

Desert::Desert(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
			   Bomberman *bomberman)
	:  // Camera(pos, eulerAngles),
	  SceneTools(21, 21, pos, eulerAngles, bomberman, "Desert", "Credits"),
	  _timer(181),
	  _cooldown(0.0f) {
	configAI();
}

Desert::~Desert(void) {}

void Desert::configAI(void) {
	_staticDecor.push_back("Wall");

	_tmpDecor.push_back("Explosion");
	_tmpDecor.push_back("OFDT");
	_tmpDecor.push_back("RunAway");
	_tmpDecor.push_back("EPB");
	_tmpDecor.push_back("Basic");
	_tmpDecor.push_back("Bomb");
	_tmpDecor.push_back("Box");

	_tmpDecorForRunAway.push_back("Explosion");
	_tmpDecorForRunAway.push_back("Bomb");
	_tmpDecorForRunAway.push_back("Box");
}

void Desert::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImages(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_refreshAI = false;
	_pauseMenu = false;
}

void Desert::drawGUI(GUI *graphicUI) {
	if (!_debugMode && (_pauseMenu || _gameEngine->isKeyPressed(KEY_ESCAPE))) {
		_pauseMenu = _displayPauseMenu(graphicUI);
		_isPause = _pauseMenu;
	}

	if (_showPlayerHp) {
		if (_showVictoryScreen) {
			_displayVictoryScreen(graphicUI);
		} else if (_showDeathScreen) {
			_displayDeathScreen(graphicUI);
		}
		_displayPlayerHP(graphicUI, _playerHp);
	}

	if (!_isPause && !_showDeathScreen && !_showVictoryScreen)
		_displayTimer(graphicUI, &_timer, false);
	else
		_displayTimer(graphicUI, &_timer, true);
	if ((int)_timer == 0) _showDeathScreen = true;
	// static int searchWord = 0;
	// static int lastWord = 0;
	// static int startStrIdx = 0;
	// std::string str =
	// 	"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
	// 	"officia deserunt mollit anim id est laborum.";
	// _displayDialogue(graphicUI, &searchWord, &lastWord, &startStrIdx,
	// 				 "Bomberman", "heart", str, false, 1000, 1000, NK_TEXT_LEFT,
	// 				 "12_BOMBERMAN", "18_BOMBERMAN");
}

void Desert::tellPosition(Entity *entity) { _savePositions(entity); }

void Desert::update(void) {
	Camera::update();
	_refreshAI = false;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.1f;
		_startBuildingGrapheForPathFinding();
		_refreshAI = true;
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
