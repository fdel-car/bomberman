#include "game/Bomberman.hpp"
#include "game/entities/Box.hpp"
#include "game/entities/EnemyBasic.hpp"
#include "game/entities/EnemyOFDT.hpp"
#include "game/entities/EnemyPutBomb.hpp"
#include "game/entities/EnemyRunAway.hpp"
#include "game/entities/Perk.hpp"
#include "game/entities/Player.hpp"
#include "game/entities/Portal.hpp"
#include "game/scenes/Desert.hpp"
#include "game/scenes/Forest.hpp"
#include "game/scenes/MainMenu.hpp"
#include "game/scenes/Volcano.hpp"

extern std::string _assetsDir;

Bomberman::Bomberman(void) : AGame(11), _startLevelName("MainMenu") {
	// Set needed fonts
	for (float size = 12.0f; size <= 48.0f; size += 1.0f)
		_neededFonts.push_back(std::tuple<float, std::string, std::string>(
			size, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));

	// Set collision table
	setLayerCollision(WallLayer, WallLayer, false);
	setLayerCollision(WallLayer, BoxLayer, false);
	setLayerCollision(WallLayer, ExplosionLayer, false);
	setLayerCollision(WallLayer, PerkLayer, false);

	setLayerCollision(PlayerLayer, EnemySpecialLayer, false);

	setLayerCollision(PlayerSpecialLayer, ExplosionLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemyLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemySpecialLayer, false);

	setLayerCollision(EnemyRunAwayLayer, EnemyRunAwayLayer, false);
	setLayerCollision(EnemyRunAwayLayer, EnemySpecialLayer, false);
	setLayerCollision(EnemyRunAwayLayer, EnemyLayer, false);

	setLayerCollision(EnemySpecialLayer, EnemySpecialLayer, false);
	setLayerCollision(EnemySpecialLayer, ExplosionLayer, false);

	setLayerCollision(PerkLayer, PerkLayer, false);
	setLayerCollision(PerkLayer, BoxLayer, false);
	setLayerCollision(PerkLayer, ExplosionLayer, false);
	setLayerCollision(PerkLayer, BombLayer, false);

	setLayerCollision(PortalLayer, WallLayer, false);
	setLayerCollision(PortalLayer, BoxLayer, false);
	setLayerCollision(PortalLayer, PlayerSpecialLayer, false);
	setLayerCollision(PortalLayer, EnemyRunAwayLayer, false);
	setLayerCollision(PortalLayer, EnemySpecialLayer, false);
	setLayerCollision(PortalLayer, BombLayer, false);
	setLayerCollision(PortalLayer, ExplosionLayer, false);
	setLayerCollision(PortalLayer, PerkLayer, false);
	setLayerCollision(PortalLayer, PortalLayer, false);

	// Set map of scenes
	_initScenes();
	initLoadScene();
}

Bomberman::~Bomberman(void) {}

std::string Bomberman::getStartLevelName(void) { return _startLevelName; }

void Bomberman::loadSceneByIndex(int sceneIdx, std::atomic_int *_sceneState,
								 bool *_checkLoadSceneIsGood) {
	unload();
	if (sceneIdx < 0 || sceneIdx >= static_cast<int>(_scenesNames.size()))
		*_checkLoadSceneIsGood = false;
	else {
		(this->*(_scenesMap[_scenesNames[sceneIdx]]))();
		*_checkLoadSceneIsGood = true;
	}
	*_sceneState = BACKGROUND_LOAD_FINISHED;
}

void Bomberman::initLoadScene() {
	_loadingSkybox = nullptr;
	size_t firstPlayableLvlIdx = 1;
	size_t lastPlayableLvlIdx = _scenesNames.size() - 1;
	size_t maxPlayableLvlIdx;
	if (_save.level < firstPlayableLvlIdx) {
		maxPlayableLvlIdx = firstPlayableLvlIdx + 1;
	} else {
		maxPlayableLvlIdx = _save.level;
		maxPlayableLvlIdx += (maxPlayableLvlIdx < lastPlayableLvlIdx) ? 2 : 1;
	}
	_loadingCamera = new MainMenu(
		glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0f),
		std::vector<std::string>(_scenesNames.begin() + firstPlayableLvlIdx,
								 _scenesNames.begin() + maxPlayableLvlIdx),
		this);  // TODO: Create Class for Loading scene
	_loadingLight = new Light(glm::vec2(-10.0, -10.0), glm::vec3(0.0f), 10.0f);
}

Save &Bomberman::getSave(void) { return _save; }

void Bomberman::_mainMenu(void) {
	_skybox = new Skybox("default");
	size_t firstPlayableLvlIdx = 1;
	size_t lastPlayableLvlIdx = _scenesNames.size() - 1;
	size_t maxPlayableLvlIdx;
	if (_save.level < firstPlayableLvlIdx) {
		maxPlayableLvlIdx = firstPlayableLvlIdx + 1;
	} else {
		maxPlayableLvlIdx = _save.level;
		maxPlayableLvlIdx += (maxPlayableLvlIdx < lastPlayableLvlIdx) ? 2 : 1;
	}
	_camera = new MainMenu(
		glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0f),
		std::vector<std::string>(_scenesNames.begin() + firstPlayableLvlIdx,
								 _scenesNames.begin() + maxPlayableLvlIdx),
		this);
	_light = new Light(glm::vec2(-10.0, -10.0), glm::vec3(0.0f), 10.0f);
	_entities.push_back(new Entity(glm::vec3(2.0, 0.5, -2.0), glm::vec3(0.0f),
								   nullptr, "Bomb", "Bomb", "Bomb"));
	_entities.back()->scale(glm::vec3(2.5f));
}

void Bomberman::_forest(void) {
	_skybox = new Skybox("default");
	_camera = new Forest(glm::vec3(-5.35, 20.0, 6.0),
						 glm::vec3(-60.0, 0.0, 0.0), this);
	_light = new Light(glm::vec2(-20.0, 8.0), glm::vec3(0.0f));
	_entities.push_back(new Entity(glm::vec3(0.0f), glm::vec3(0.0f), nullptr,
								   "Island", "Island", "Island"));
	_entities.push_back(new Player(glm::vec3(-9.0, 0.0, -9.0), glm::vec3(0.0f),
								   _save, _camera));

	// Portal to clear lvl
	_entities.push_back(new Portal(glm::vec3(1.0, 0, 1.0), _camera));

	// Enemies
	// _entities.push_back(
	// 	new EnemyOFDT(glm::vec3(7.0, 0.0, -7.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyRunAway(glm::vec3(7.0, 0.0, 7.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyOFDT(glm::vec3(-7.0, 0.0, 7.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyPutBomb(glm::vec3(7.0, 0.0, -7.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyRunAway(glm::vec3(-3.0, 0.0, 3.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyBasic(glm::vec3(3.0, 0.0, -3.0), glm::vec3(0.0f), _camera));
	// _entities.push_back(
	// 	new EnemyBasic(glm::vec3(3.0, 0.0, 3.0), glm::vec3(0.0f), _camera));

	// Walls/Boxes
	// size_t totalBoxes = 120;
	// size_t avgPerks = 10;
	// size_t perkProb = (avgPerks * 100) / totalBoxes;
	_createMap(10, 10);
}

void Bomberman::_desert(void) {
	_camera = new Desert(glm::vec3(-9.25, 20.0, 6.0),
						 glm::vec3(-60.0, 0.0, 0.0), this);
	_light = new Light(glm::vec2(-20.0, 8.0), glm::vec3(0.0f));
	_entities.push_back(new Entity(glm::vec3(0.0f), glm::vec3(0.0f), nullptr,
								   "Island", "Island", "Island"));
	_entities.push_back(new Player(glm::vec3(-1.0, 0.0, -1.0), glm::vec3(0.0f),
								   _save, _camera));

	// Portal to clear lvl
	_entities.push_back(new Portal(glm::vec3(0, 0, -7), _camera));
	// _createMap(18, 18);
}

void Bomberman::_volcano(void) {
	_camera = new Volcano(glm::vec3(-14.5, 20.0, -3.0),
						  glm::vec3(-60.0, 0.0, 0.0), this);
	_light = new Light(glm::vec2(-20.0, 8.0), glm::vec3(0.0f));
	_entities.push_back(new Entity(glm::vec3(0.0f), glm::vec3(0.0f), nullptr,
								   "Island", "Island", "Island"));
	_entities.push_back(new Player(glm::vec3(-17.0, 0.0, -17.0),
								   glm::vec3(0.0f), _save, _camera));

	// Portal to clear lvl
	_entities.push_back(new Portal(glm::vec3(-7, 0, 0), _camera));
	_createMap(18, 18);
}

void Bomberman::_initScenes(void) {
	_scenesNames.push_back(_startLevelName);
	_scenesMap[_scenesNames.back()] = &Bomberman::_mainMenu;

	_scenesNames.push_back("Forest");
	_scenesMap[_scenesNames.back()] = &Bomberman::_forest;
	_scenesNames.push_back("Volcano");
	_scenesMap[_scenesNames.back()] = &Bomberman::_volcano;
	_scenesNames.push_back("Desert");
	_scenesMap[_scenesNames.back()] = &Bomberman::_desert;
}

void Bomberman::_createMap(int width, int height) {
	for (int x = -width; x <= width; x++) {
		for (int z = -height; z <= height; z++) {
			if (abs(x) == width || abs(z) == height) {
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
			}
			//  else if (x != -7 && z != -7 && x != 7 && z != 7 && x % 2 != 0 &&
			//    z % 2 != 0) {
			// _entities.push_back(new Box(glm::vec3(x, 0, z), _camera));
			// }
		}
	}
}

size_t Bomberman::getWindowWidth() {
	return std::get<1>(Save::RESOLUTIONS[_save.resolutionsIdx]);
}
size_t Bomberman::getWindowHeight() {
	return std::get<2>(Save::RESOLUTIONS[_save.resolutionsIdx]);
}

int Bomberman::getLoadingSceneIdx(void) const {
	return getSceneIndexByName("LoadScene");
}

int Bomberman::getFirstSceneIdx(void) const {
	return getSceneIndexByName(_startLevelName);
}
bool Bomberman::isFullScreen() {
	return std::get<1>(Save::FULL_SCREEN[_save.isFullScreen]);
}
