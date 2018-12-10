#include "game/scenes/Space.hpp"
#include "engine/GameEngine.hpp"

extern std::string _assetsDir;

Space::Space(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
			 Bomberman *bomberman)
	:  // Camera(pos, eulerAngles),
	  SceneTools(29, 21, pos, eulerAngles, bomberman, "Space", "Credits"),
	  _cooldown(0.0f) {
	configAI();
	_initSoundsForGameplay();
	_startMusic = _assetsDir + "Audio/Musics/Mystery.wav";
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
}

void Space::drawGUI(GUI *graphicUI) { SceneTools::drawGUI(graphicUI); }

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
