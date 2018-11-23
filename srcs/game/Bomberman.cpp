#include "game/Bomberman.hpp"
#include "game/cams/ForestCam.hpp"
#include "game/cams/MainMenuCam.hpp"
#include "game/entities/Enemy.hpp"
#include "game/entities/Player.hpp"

#include "json/json.hpp"  // https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp

extern std::string _assetsDir;

Bomberman::Bomberman(void) : AGame(5) {
	// Set needed fonts
	for (float size = 12.0f; size <= 48.0f; size += 1.0f)
		_neededFonts.push_back(std::tuple<float, std::string, std::string>(
			size, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	// Set collision table
	_collisionTable[PlayerLayer][BombLayer] = false;
	// _collisionTable[WallLayer][PlayerLayer] = false;
	// Set map of scenes
	_initScenes();
}

Bomberman::~Bomberman(void) {}

bool Bomberman::loadSceneByIndex(int sceneIdx) {
	unload();
	if (sceneIdx < 0 || sceneIdx >= static_cast<int>(_scenesNames.size()))
		return false;
	(this->*(_scenesMap[_scenesNames[sceneIdx]]))();
	return true;
}

void Bomberman::_mainMenu(void) {
	_camera = new MainMenuCam(
		glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0f),
		std::vector<std::string>(_scenesNames.begin() + 1, _scenesNames.end()),
		_save);
	_entities.push_back(new Entity(glm::vec3(2.0, 0.5, -2.0), glm::vec3(0.0f),
								   nullptr, "Bomb", "Bomb", "Bomb"));
	_entities.back()->scale(glm::vec3(5.0));
}

void Bomberman::_forest(void) {
	_camera =
		new ForestCam(glm::vec3(0.0, 34.0, 20.0), glm::vec3(-60.0, 0.0, 0.0));
	_entities.push_back(new Entity(glm::vec3(0.0f), glm::vec3(0.0f), nullptr,
								   "Island", "Island", "Island"));
	_entities.push_back(new Player(glm::vec3(-7.0, 0.0, -7.0), glm::vec3(0.0f),
								   _save, _camera));
	_entities.push_back(
		new Enemy(glm::vec3(7.0, 0.0, 7.0), glm::vec3(0.0f), _camera));
	for (int x = -8; x <= 8; x++) {
		for (int z = -8; z <= 8; z++) {
			if ((x % 2 == 0 && z % 2 == 0) || abs(x) == 8 || abs(z) == 8) {
				_entities.push_back(
					new Entity(glm::vec3(x, 0.5, z), glm::vec3(0.0f),
							   new Collider(Collider::Rectangle,
											LayerTag::WallLayer, 0.5, 0.5),
							   "Box", "Box", "Box", _camera));
				_entities.back()->scale(glm::vec3(1.0, 0.75, 1.0));
			}
		}
	}
}

void Bomberman::_initScenes(void) {
	_scenesNames.push_back("MainMenu");
	_scenesMap[_scenesNames.back()] = &Bomberman::_mainMenu;
	_scenesNames.push_back("Forest");
	_scenesMap[_scenesNames.back()] = &Bomberman::_forest;
}

size_t Bomberman::getWindowWidth() {
	return std::get<1>(Save::RESOLUTIONS[_save.resolutionsIdx]);
}
size_t Bomberman::getWindowHeight() {
	return std::get<2>(Save::RESOLUTIONS[_save.resolutionsIdx]);
}
bool Bomberman::isFullScreen() {
	return std::get<1>(Save::FULL_SCREEN[_save.isFullScreen]);
}
