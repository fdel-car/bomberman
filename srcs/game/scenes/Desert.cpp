#include "game/scenes/Desert.hpp"
#include "engine/GameEngine.hpp"

Desert::Desert(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
			   Bomberman *bomberman)
	:  // Camera(pos, eulerAngles),
	  SceneTools(41, 9, pos, eulerAngles, bomberman, "Desert", "Credits"),
	  _cooldown(0.0f) {
	configAI();
	_initSoundsForGameplay();
	_startMusic = "";  // TODO
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

	_tmpDecorForRunAway.push_back("Player");
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
}

void Desert::drawGUI(GUI *graphicUI) {
	SceneTools::drawGUI(graphicUI);
}

void Desert::tellPosition(Entity *entity) { _savePositions(entity); }

void Desert::update(void) {
	Camera::update();
	_refreshAI = false;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.2f;
		_startBuildingGrapheForPathFinding();
		_refreshAI = true;
		// printMapInfo();
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
