#include "bomberman/BombermanGame.hpp"
#include "bomberman/cams/ForestCam.hpp"
#include "bomberman/cams/MainMenuCam.hpp"
#include "bomberman/entities/Enemy.hpp"
#include "bomberman/entities/Player.hpp"

#include "json/json.hpp"  // https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp

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
		std::vector<std::string>(_scenesNames.begin() + 1, _scenesNames.end()),
		save);
	_entities.push_back(new Entity(glm::vec3(6.0, 1.5, 0.0),
								   glm::vec3(0.0, 0.0, 0.0f), NULL, "Bomb"));
	_entities.back()->scale(glm::vec3(10.0));
}

void BombermanGame::_forest(void) {
	_camera = new ForestCam(glm::vec3(0.0f, 28.0f, 16.8f),
							glm::vec3(-60.0f, 0.0f, 0.0f));
	// _entities.push_back(new Entity(glm::vec3(0.0, 0.0, 0.0),
	// 							   glm::vec3(0.0, 0.0, 0.0f), nullptr,
	// 							   "Island"));
	_entities.push_back(new Player(glm::vec3(-7.0f, 0.5f, -7.0f),
								   glm::vec3(0.0f, 0.0f, 0.0f), save, _camera));
	_entities.push_back(new Enemy(glm::vec3(7.0f, 0.5f, 7.0f),
								  glm::vec3(0.0f, 0.0f, 0.0f), _camera));
	_entities.push_back(new Entity(
		glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0f), nullptr, "Floor"));
	_entities.back()->scale(glm::vec3(0.3f, 0.3f, 0.3f));
	for (int x = -8; x <= 8; x++) {
		for (int z = -8; z <= 8; z++) {
			if ((x % 2 == 0 && z % 2 == 0) || abs(x) == 8 || abs(z) == 8) {
				_entities.push_back(
					new Entity(glm::vec3(x, 0.5f, z), glm::vec3(0.0, 0.0, 0.0f),
							   new Collider(Collider::Rectangle, 0.5f, 0.5f),
							   "Box", _camera));
				_entities.back()->scale(glm::vec3(1.0f, 0.75f, 1.0f));
			}
		}
	}

	// Few trees
	_entities.push_back(new Entity(glm::vec3(-12.0, 0.0, -4.0),
								   glm::vec3(0.0, 0.0, 0.0f), nullptr, "Tree"));
	_entities.push_back(new Entity(glm::vec3(11.0, 0.0, 2.0),
								   glm::vec3(0.0, 0.0, 0.0f), nullptr, "Tree"));
	_entities.back()->rotateY(95.0f);
}

void BombermanGame::_initScenes(void) {
	_scenesNames.push_back("MainMenu");
	_scenesMap[_scenesNames.back()] = &BombermanGame::_mainMenu;
	_scenesNames.push_back("Forest");
	_scenesMap[_scenesNames.back()] = &BombermanGame::_forest;
}

size_t BombermanGame::getWindowWidth() {
	return std::get<1>(Save::RESOLUTIONS[save.resolutionsIdx]);
}
size_t BombermanGame::getWindowHeight() {
	return std::get<2>(Save::RESOLUTIONS[save.resolutionsIdx]);
}
bool BombermanGame::isFullScreen() {
	return std::get<1>(Save::FULL_SCREEN[save.isFullScreen]);
}
