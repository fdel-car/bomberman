#include "scenes/BombermanGame.hpp"
#include "Player.hpp"
#include "scenes/ForestCam.hpp"
#include "scenes/MainMenuCam.hpp"

extern std::string _assetsDir;

BombermanGame::BombermanGame(void) : AGame() {
	for (float size = 12.0f; size <= 48.0f; size += 1.0f)
		_neededFonts.push_back(std::tuple<float, std::string, std::string>(
			size, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_initScenes();
}

BombermanGame::~BombermanGame(void) {}

bool BombermanGame::loadSceneByIndex(int sceneIdx) {
	unload();
	if (sceneIdx < 0 || sceneIdx >= static_cast<int>(_scenesNames.size()))
		return false;
	(this->*(_scenesMap[_scenesNames[sceneIdx]]))();
	return true;
}

void BombermanGame::_mainMenu(void) {
	_camera = new MainMenuCam(
		glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0, 0.0, 0.0),
		std::vector<std::string>(_scenesNames.begin() + 1, _scenesNames.end()));
	_entities.push_back(new Entity(glm::vec3(6.0, 1.5, 0.0),
								   glm::vec3(0.0, 0.0, 0.0f), NULL, "Bomb"));
	_entities.back()->scale(glm::vec3(10.0));
}

void BombermanGame::_forest(void) {
	_camera = new ForestCam(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0));
	// _entities.push_back(
	// new Player(glm::vec3(2.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0f)));
	_entities.push_back(new Entity(glm::vec3(0.0, 0.0, 0.0),
								   glm::vec3(0.0, 0.0, 0.0f), nullptr, "Tree"));
	// _entities.push_back(new Entity(glm::vec3(2.0, 0.0, 3.0),
	// 							   glm::vec3(0.0, 0.0, 0.0f), nullptr, "Tree"));
}

void BombermanGame::_initScenes(void) {
	_scenesNames.push_back("MainMenu");
	_scenesMap[_scenesNames.back()] = &BombermanGame::_mainMenu;
	_scenesNames.push_back("Forest");
	_scenesMap[_scenesNames.back()] = &BombermanGame::_forest;
}