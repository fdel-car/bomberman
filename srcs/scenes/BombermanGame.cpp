#include "scenes/BombermanGame.hpp"
#include "Player.hpp"
#include "scenes/ForestCam.hpp"
#include "scenes/MainMenuCam.hpp"

extern std::string _assetsDir;

BombermanGame::BombermanGame(void) : AGame() {
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		48.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		47.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		46.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		45.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		44.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		43.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		42.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		41.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		40.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		30.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		29.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		28.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		27.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		26.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		25.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		24.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		23.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		22.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		21.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		20.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		19.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		18.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		17.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		16.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		15.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		14.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		13.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_neededFonts.push_back(std::tuple<float, std::string, std::string>(
		12.0f, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
}

BombermanGame::~BombermanGame(void) {}

bool BombermanGame::loadScene(size_t sceneIdx) {
	unload();
	if (sceneIdx >= _scenes.size()) return false;
	return (this->*(_scenes[sceneIdx]))();
}

bool BombermanGame::_mainMenu(void) {
	_camera =
		new MainMenuCam(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0, 0.0, 0.0));
	_entities.push_back(new Entity(glm::vec3(6.0, 1.5, 0.0),
								   glm::vec3(0.0, 0.0, 0.0f), NULL, "Bomb"));
	_entities.back()->scale(glm::vec3(10.0));
	return true;
}

bool BombermanGame::_forest(void) {
	_camera =
		new ForestCam(glm::vec3(0.0, 10.0, 3.0), glm::vec3(-75.0, 0.0, 0.0));
	_entities.push_back(
		new Player(glm::vec3(2.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0f)));
	_entities.push_back(
		new Entity(glm::vec3(-2.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0f),
				   new Collider(Collider::Rectangle, 1.0f, 1.0f), "Cube"));
	return true;
}

std::vector<BombermanGame::Scene> BombermanGame::_initScenes(void) {
	std::vector<BombermanGame::Scene> scenes;
	scenes.push_back(&BombermanGame::_mainMenu);
	scenes.push_back(&BombermanGame::_forest);
	return scenes;
}

std::vector<BombermanGame::Scene> BombermanGame::_scenes =
	BombermanGame::_initScenes();