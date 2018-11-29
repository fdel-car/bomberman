#include "game/Bomberman.hpp"
#include "game/entities/Box.hpp"
#include "game/entities/EnemyOFDT.hpp"
#include "game/entities/EnemyRunAway.hpp"
#include "game/entities/Perk.hpp"
#include "game/entities/Player.hpp"
#include "game/scenes/Forest.hpp"
#include "game/scenes/MainMenu.hpp"

extern std::string _assetsDir;

Bomberman::Bomberman(void) : AGame(10) {
	// Set needed fonts
	for (float size = 12.0f; size <= 48.0f; size += 1.0f)
		_neededFonts.push_back(std::tuple<float, std::string, std::string>(
			size, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));

	// Set collision table
	setLayerCollision(WallLayer, WallLayer, false);
	setLayerCollision(WallLayer, BoxLayer, false);
	setLayerCollision(WallLayer, ExplosionLayer, false);
	setLayerCollision(WallLayer, BombLayer, false);
	setLayerCollision(WallLayer, PerkLayer, false);

	setLayerCollision(PlayerLayer, EnemySpecialLayer, false);
	setLayerCollision(EnemyRunAwayLayer, EnemySpecialLayer, false);
	setLayerCollision(EnemyRunAwayLayer, EnemyLayer, false);
	setLayerCollision(PlayerSpecialLayer, ExplosionLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemyLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemySpecialLayer, false);

	setLayerCollision(EnemySpecialLayer, ExplosionLayer, false);

	setLayerCollision(PerkLayer, BoxLayer, false);
	setLayerCollision(PerkLayer, ExplosionLayer, false);
	setLayerCollision(PerkLayer, BombLayer, false);

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
	_camera = new MainMenu(
		glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0f),
		std::vector<std::string>(_scenesNames.begin() + 1, _scenesNames.end()),
		_save);
	_light = new Light(glm::vec2(-10.0, -10.0), glm::vec3(0.0f), 10.0f);
	_entities.push_back(new Entity(glm::vec3(2.0, 0.5, -2.0), glm::vec3(0.0f),
								   nullptr, "Bomb", "Bomb", "Bomb"));
	_entities.back()->scale(glm::vec3(5.0));
}

void Bomberman::_forest(void) {
	_camera =
		new Forest(glm::vec3(0.0, 34.0, 20.0), glm::vec3(-60.0, 0.0, 0.0));
	_light = new Light(glm::vec2(-20.0, 8.0), glm::vec3(0.0f));
	_entities.push_back(new Entity(glm::vec3(0.0f), glm::vec3(0.0f), nullptr,
								   "Island", "Island", "Island"));
	_entities.push_back(new Player(glm::vec3(-7.0, 0.0, -7.0), glm::vec3(0.0f),
								   _save, _camera));
	// _entities.push_back(new Perk(glm::vec3(-6.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(-5.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(-4.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(-3.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(-2.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(-1.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(0.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(5.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(4.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(3.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(2.0, 0.0, -7.0), _camera));
	// _entities.push_back(new Perk(glm::vec3(1.0, 0.0, -7.0), _camera));

	// Enemies
	_entities.push_back(
		new EnemyOFDT(glm::vec3(-7.0, 0.0, 7.0), glm::vec3(0.0f), _camera));
	_entities.push_back(
		new EnemyRunAway(glm::vec3(7.0, 0.0, 7.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyOFDT(glm::vec3(-7.0, 0.0, 7.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyRunAway(glm::vec3(-3.0, 0.0, 3.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyOFDT(glm::vec3(3.0, 0.0, -3.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyOFDT(glm::vec3(3.0, 0.0, 3.0), glm::vec3(0.0f), _camera));

	// Walls/Boxes
	size_t totalBoxes = 120;
	size_t avgPerks = 60;
	size_t perkProb = (avgPerks * 100) / totalBoxes;
	for (int x = -8; x <= 8; x++) {
		for (int z = -8; z <= 8; z++) {
			if (abs(x) == 8 || abs(z) == 8) {
				_entities.push_back(
					new Entity(glm::vec3(x, 0.5, z), glm::vec3(0.0f),
							   new Collider(Collider::Rectangle,
											LayerTag::WallLayer, 0.5, 0.5),
							   "Wall", "Wall", "Wall", _camera));
				_entities.back()->scale(glm::vec3(0.9, 1.0, 0.9));
			} else if (x % 2 == 0 && z % 2 == 0) {
				_entities.push_back(
					new Entity(glm::vec3(x, 0.4, z), glm::vec3(0.0f),
							   new Collider(Collider::Rectangle,
											LayerTag::WallLayer, 0.5, 0.5),
							   "Wall", "Wall", "Wall", _camera));
				_entities.back()->scale(glm::vec3(1.0, 0.8, 1.0));
			} else if (x != -7 && z != -7 && x != 7 && z != 7) {
				_entities.push_back(new Box(glm::vec3(x, 0, z), _camera));
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
