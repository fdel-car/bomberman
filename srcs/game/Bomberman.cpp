#include "game/Bomberman.hpp"
#include "game/entities/Box.hpp"
#include "game/entities/EnemyBasic.hpp"
#include "game/entities/EnemyBomber.hpp"
#include "game/entities/EnemyMelee.hpp"
#include "game/entities/EnemyRunAway.hpp"
#include "game/entities/Perk.hpp"
#include "game/entities/Player.hpp"
#include "game/entities/Portal.hpp"
#include "game/scenes/Credits.hpp"
#include "game/scenes/Forest.hpp"
#include "game/scenes/Load.hpp"
#include "game/scenes/MainMenu.hpp"
#include "game/scenes/Mario.hpp"
#include "game/scenes/Pokemon.hpp"
#include "game/scenes/Space.hpp"

extern std::string _assetsDir;

Bomberman::Bomberman(void) : AGame(14), _startLevelName("MainMenu") {
	// Set needed fonts
	for (float size = 12.0f; size <= 48.0f; size += 1.0f)
		_neededFonts.push_back(std::tuple<float, std::string, std::string>(
			size, (_assetsDir + "GUI/Fonts/slider.ttf"), "slider"));

	// Set collision table
	setLayerCollision(WallLayer, WallLayer, false);
	setLayerCollision(WallLayer, BoxLayer, false);
	setLayerCollision(WallLayer, ExplosionLayer, false);
	setLayerCollision(WallLayer, PerkLayer, false);

	setLayerCollision(PlayerLayer, EnemySpecialLayer, false);

	setLayerCollision(PlayerSpecialLayer, ExplosionLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemyMeleeLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemyLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemySpecialLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemyBasicLayer, false);
	setLayerCollision(PlayerSpecialLayer, EnemyRunAwayLayer, false);

	setLayerCollision(EnemyRunAwayLayer, EnemyRunAwayLayer, false);
	setLayerCollision(EnemyBomberLayer, EnemyBomberLayer, false);
	setLayerCollision(EnemyBomberLayer, EnemyBasicLayer, false);
	setLayerCollision(EnemyRunAwayLayer, EnemySpecialLayer, false);
	setLayerCollision(EnemyRunAwayLayer, EnemyLayer, false);

	setLayerCollision(EnemySpecialLayer, EnemySpecialLayer, false);
	setLayerCollision(EnemySpecialLayer, ExplosionLayer, false);

	setLayerCollision(EnemyBasicLayer, EnemyBasicLayer, false);
	setLayerCollision(EnemyRunAwayLayer, EnemyBasicLayer, false);
	setLayerCollision(EnemyMeleeLayer, EnemyMeleeLayer, false);

	setLayerCollision(PerkLayer, PerkLayer, false);
	setLayerCollision(PerkLayer, BoxLayer, false);
	setLayerCollision(PerkLayer, ExplosionLayer, false);
	setLayerCollision(PerkLayer, BombLayer, false);

	setLayerCollision(PortalLayer, WallLayer, false);
	setLayerCollision(PortalLayer, BoxLayer, false);
	setLayerCollision(PortalLayer, PlayerSpecialLayer, false);
	setLayerCollision(PortalLayer, EnemyRunAwayLayer, false);
	setLayerCollision(PortalLayer, EnemyLayer, false);
	setLayerCollision(PortalLayer, EnemySpecialLayer, false);
	setLayerCollision(PortalLayer, BombLayer, false);
	setLayerCollision(PortalLayer, ExplosionLayer, false);
	setLayerCollision(PortalLayer, EnemyBomberLayer, false);
	setLayerCollision(PortalLayer, PerkLayer, false);
	setLayerCollision(PortalLayer, PortalLayer, false);
	setLayerCollision(PortalLayer, EnemyMeleeLayer, false);

	// Set all assets that will be needed by game (they wont be charged until
	// added to _neededAssets)

	// Set map of scenes
	_initScenes();
	initLoadScene();
}

Bomberman::~Bomberman(void) {}

std::string Bomberman::getStartLevelName(void) { return _startLevelName; }

int Bomberman::getStartingMusicVolume(void) const { return _save.musicVolume; }

int Bomberman::getStartingSoundsVolume(void) const {
	return _save.soundsVolume;
}

void Bomberman::loadSceneByIndex(int sceneIdx, std::atomic_int *_sceneState,
								 bool *_checkLoadSceneIsGood) {
	unload();
	Perk::kickPerkDropped = false;
	if (sceneIdx < 0 || sceneIdx >= static_cast<int>(_scenesNames.size()))
		*_checkLoadSceneIsGood = false;
	else {
		(this->*(_scenesMap[_scenesNames[sceneIdx]]))();
		*_checkLoadSceneIsGood = true;
		AGame::loadAssets();
		AGame::loadSounds();
	}
	*_sceneState = BACKGROUND_LOAD_FINISHED;
}

void Bomberman::initLoadScene() {
	WorldLocation startLocation(glm::vec3(-5.35, 20.0, 6.0),
								glm::vec3(-60.0, 0.0, 0.0));
	_loadingCamera = new Load(startLocation, this);
	_loadingLight = new Light(glm::vec2(-10.0, -10.0), glm::vec3(0.0f), 10.0f);
}

void Bomberman::initAllAssets(void) {
	std::map<std::string, std::string> animInfo;
	_allAssets["Sphere"] = ModelInfo("Models/Sphere/sphere.dae");
	_allAssets["Bomb"] = ModelInfo("Models/Bomb/bomb.dae");
	_allAssets["Island"] = ModelInfo("Models/Island/island.obj");
	_allAssets["Stadium"] = ModelInfo("Models/Stadium/stadium.obj");
	_allAssets["Wall"] = ModelInfo("Models/Wall/wall.obj");
	_allAssets["Box"] = ModelInfo("Models/Box/box.obj");
	_allAssets["Portal"] = ModelInfo("Models/Portal/portal.obj");
	animInfo["Run"] = "Animations/Hero/run.dae";
	_allAssets["Player"] = ModelInfo("Models/Hero/hero.dae", animInfo);
	animInfo.clear();
	_allAssets["KickPerk"] = ModelInfo("Models/Perks/Kick/kick.obj");
	_allAssets["DamagePerk"] = ModelInfo("Models/Perks/Damage/damage.obj");
	_allAssets["MaxBombPerk"] = ModelInfo("Models/Perks/MaxBomb/maxBomb.obj");
	_allAssets["RangePerk"] = ModelInfo("Models/Perks/Range/range.obj");
	_allAssets["SpeedPerk"] = ModelInfo("Models/Perks/Speed/speed.obj");
	_allAssets["Meteor"] = ModelInfo("Models/Meteorite/meteorite.obj");
	_allAssets["BigMeteor"] = ModelInfo("Models/BigMeteor/bigMeteor.obj");
	_allAssets["DestructibleMeteor"] =
		ModelInfo("Models/DestructibleMeteorite/destructibleMeteorite.obj");
	_allAssets["StrengthBoulder"] =
		ModelInfo("Models/StrengthBoulder/strengthBoulder.obj");
	_allAssets["Fuzzy"] = ModelInfo("Models/Fuzzy/fuzzy.obj");
	_allAssets["Diglett"] = ModelInfo("Models/Diglett/diglett.obj");
	_allAssets["Lapras"] = ModelInfo("Models/Lapras/lapras.obj");
	_allAssets["Groudon"] = ModelInfo("Models/Groudon/groudon.obj");
	_allAssets["RedGhost"] = ModelInfo("Models/RedGhost/redGhost.obj");
	_allAssets["EnemyBomber"] = ModelInfo("Models/EnemyBomber/enemyBomber.obj");
	_allAssets["DomeFossil"] =
		ModelInfo("Models/Fossils/DomeFossil/domeFossil.obj");
	_allAssets["HelixFossil"] =
		ModelInfo("Models/Fossils/HelixFossil/helixFossil.obj");
	_allAssets["OakTree"] = ModelInfo("Models/OakTree/oakTree.obj");
	_allAssets["WarpPipe"] = ModelInfo("Models/WarpPipe/WarpPipe.obj");
	_allAssets["WarpPipeCorner"] =
		ModelInfo("Models/WarpPipeCorner/WarpPipeCorner.obj");
	_allAssets["RockPipe"] = ModelInfo("Models/RockPipe/RockPipe.obj");
	_allAssets["Pipe"] = ModelInfo("Models/Pipe/Pipe.obj");
	_allAssets["CloudMap"] = ModelInfo("Models/CloudMap/CloudMap.obj");
}

void Bomberman::initAllSounds(void) {
	// Main menu
	_allSounds["select"] = "Audio/Sounds/Menu/select.wav";
	_allSounds["lateral_select"] = "Audio/Sounds/Menu/lateral_select.wav";
	_allSounds["first_load"] = "Audio/Sounds/Menu/first_load.wav";
	_allSounds["open_settings"] = "Audio/Sounds/Menu/open_settings.wav";
	_allSounds["back"] = "Audio/Sounds/Menu/back.wav";
	_allSounds["reset_default"] = "Audio/Sounds/Menu/reset_default.wav";
	_allSounds["save"] = "Audio/Sounds/Menu/save.wav";

	// In-Game
	_allSounds["explosion_1"] = "Audio/Sounds/Explosion/explosion_1.wav";
	_allSounds["explosion_2"] = "Audio/Sounds/Explosion/explosion_2.wav";
	_allSounds["explosion_with_box_1"] =
		"Audio/Sounds/Explosion/explosion_with_box_1.wav";
	_allSounds["explosion_with_box_2"] =
		"Audio/Sounds/Explosion/explosion_with_box_2.wav";
	_allSounds["time_over_effect"] = "Audio/Sounds/Hero/time_over_effect.wav";
	_allSounds["time_over_voice"] = "Audio/Sounds/Hero/time_over_voice.wav";
	_allSounds["winner_effect"] = "Audio/Sounds/Hero/winner_effect.wav";
	_allSounds["winner_voice"] = "Audio/Sounds/Hero/winner_voice.wav";
	_allSounds["defeat_effect"] = "Audio/Sounds/Hero/defeat_effect.wav";
	_allSounds["defeat_voice"] = "Audio/Sounds/Hero/defeat_voice.wav";
	_allSounds["pause_start"] = "Audio/Sounds/Menu/pause_start.wav";
	_allSounds["pause_end"] = "Audio/Sounds/Menu/pause_end.wav";
	_allSounds["put_bomb_1"] = "Audio/Sounds/Bomb/put_bomb_1.wav";
	_allSounds["put_bomb_2"] = "Audio/Sounds/Bomb/put_bomb_2.wav";
	_allSounds["burn_player_1"] = "Audio/Sounds/Explosion/burn_player_1.wav";
	_allSounds["burn_player_2"] = "Audio/Sounds/Explosion/burn_player_2.wav";
	_allSounds["get_perk_1"] = "Audio/Sounds/Perk/get_perk_1.wav";
	_allSounds["get_perk_2"] = "Audio/Sounds/Perk/get_perk_2.wav";
	_allSounds["get_perk_3"] = "Audio/Sounds/Perk/get_perk_3.wav";
	_allSounds["get_perk_4"] = "Audio/Sounds/Perk/get_perk_4.wav";
	_allSounds["bad_perk"] = "Audio/Sounds/Perk/bad_perk.wav";
	_allSounds["portal_spawn"] = "Audio/Sounds/Portal/portal_spawn.wav";
	_allSounds["damage_1"] = "Audio/Sounds/Enemy/damage_1.wav";
	_allSounds["damage_2"] = "Audio/Sounds/Enemy/damage_2.wav";
}

Save &Bomberman::getSave(void) { return _save; }

void Bomberman::_mainMenu(void) {
	_skybox = new Skybox("Default");
	size_t firstPlayableLvlIdx = 1;
	size_t lastPlayableLvlIdx = _scenesNames.size() - 2;
	size_t maxPlayableLvlIdx;
	if (_save.level < firstPlayableLvlIdx) {
		maxPlayableLvlIdx = firstPlayableLvlIdx + 1;
	} else {
		maxPlayableLvlIdx = (_save.level >= lastPlayableLvlIdx)
								? lastPlayableLvlIdx + 1
								: _save.level + 2;
	}
	WorldLocation startLocation(glm::vec3(-14.5, 20.0, -3.0),
								glm::vec3(-60.0, 0.0, 0.0));
	_camera = new MainMenu(
		startLocation,
		std::vector<std::string>(_scenesNames.begin() + firstPlayableLvlIdx,
								 _scenesNames.begin() + maxPlayableLvlIdx),
		this);
	_light = new Light(glm::vec2(-10.0, -10.0), glm::vec3(0.0f), 10.0f);
	// _entities.push_back(new Entity(glm::vec3(2.0, 0.5, -2.0),
	// glm::vec3(0.0f), 							   nullptr, "Bomb", "Bomb",
	// "Bomb")); _entities.back()->scale(glm::vec3(2.5f));
}

void Bomberman::_forest(void) {
	_skybox = new Skybox("Default");
	WorldLocation dialogueLocation(glm::vec3(23, 11.0, 26),
								   glm::vec3(-14.0, 37.0, 0.0));
	WorldLocation gameplayLocation(glm::vec3(-4, 20.0, 6.0),
								   glm::vec3(-60.0, 0.0, 0.0));
	_camera = new Forest(dialogueLocation, gameplayLocation, 3.0f, this);
	_light = new Light(glm::vec2(-20.0, 8.0), glm::vec3(0.0f));
	_entities.push_back(new Entity(glm::vec3(0.0f), glm::vec3(0.0f), nullptr,
								   "Island", "Island", "Island"));
	_entities.push_back(new Entity(glm::vec3(-15.0, -1.0, -9.0),
								   glm::vec3(0.0f), nullptr, "OakTree",
								   "OakTree", "OakTree"));
	_entities.back()->scale(glm::vec3(1.8f));
	_entities.push_back(new Player(glm::vec3(-7.0, 0.0, -7.0), glm::vec3(0.0f),
								   _save, _camera));

	// Portal to clear lvl
	Entity *portal = new Portal(glm::vec3(0), _camera);
	_entities.push_back(new Box(glm::vec3(7, 0, 7), _camera, "Box", 0, portal));
	std::vector<std::tuple<int, int>> protectedCase;
	protectedCase.push_back(std::tuple<int, int>(-7.0, -7.0));
	protectedCase.push_back(std::tuple<int, int>(-6.0, -7.0));
	protectedCase.push_back(std::tuple<int, int>(-7.0, -6.0));
	protectedCase.push_back(std::tuple<int, int>(-5.0, -7.0));
	protectedCase.push_back(std::tuple<int, int>(-7.0, -5.0));
	protectedCase.push_back(std::tuple<int, int>(7.0, 7.0));
	_entities.push_back(new Box(glm::vec3(-5.0, 0, -7.0), _camera, "Box"));
	_entities.push_back(new Box(glm::vec3(-7.0, 0, -5.0), _camera, "Box"));

	std::vector<std::string> border;
	border.push_back("Wall");
	std::vector<std::string> undestructibleBlock;
	undestructibleBlock.push_back("Wall");
	std::vector<std::string> destructibleBlock;
	destructibleBlock.push_back("Box");
	std::vector<std::string> enemies;
	enemies.push_back("Fuzzy");
	_createMap(8, 8, protectedCase, 6, 10, border, undestructibleBlock,
			   destructibleBlock, enemies);

	// Droppped Assets
	_neededAssets.insert("Bomb");
	_neededAssets.insert("Sphere");
	_neededAssets.insert("Portal");
	_neededAssets.insert("KickPerk");
	_neededAssets.insert("DamagePerk");
	_neededAssets.insert("MaxBombPerk");
	_neededAssets.insert("RangePerk");
	_neededAssets.insert("SpeedPerk");

	// Dropped sounds
	_neededSounds.insert("portal_spawn");
}

void Bomberman::_pokemon(void) {
	_skybox = new Skybox("Default");
	WorldLocation dialogueLocation(glm::vec3(15.5, 13.5, 33.0),
								   glm::vec3(-16.0, 24.1, 0.0));
	WorldLocation gameplayLocation(glm::vec3(-4, 20, 20),
								   glm::vec3(-60.0, 0.0, 0.0));
	_camera = new Pokemon(dialogueLocation, gameplayLocation, 3.0f, this);
	_light = new Light(glm::vec2(-50.0, 20.0), glm::vec3(0.0f));
	_entities.push_back(new Entity(glm::vec3(0.0f), glm::vec3(0.0f), nullptr,
								   "Stadium", "Stadium", "Stadium"));
	_entities.push_back(new Player(glm::vec3(-7.0, 0.0, 11.0), glm::vec3(0.0f),
								   _save, _camera));
	_entities.push_back(new Entity(glm::vec3(0, 0, 20), glm::vec3(0, 180, 0),
								   nullptr, "Lapras", "Lapras", "Lapras"));
	_entities.push_back(new Entity(glm::vec3(0, 0, -20), glm::vec3(0), nullptr,
								   "Groudon", "Groudon", "Groudon"));

	// Portal to clear lvl
	Entity *portal = new Portal(glm::vec3(0), _camera);

	// Enemies of level
	_entities.push_back(new EnemyRunAway(glm::vec3(7.0, 0.0, -11.0),
										 glm::vec3(0.0f), "Diglett", _camera,
										 portal));

	// Walls/Boxes
	std::vector<std::tuple<int, int>> protectedCase;
	protectedCase.push_back(std::tuple<int, int>(-7.0, 11.0));
	protectedCase.push_back(std::tuple<int, int>(-6.0, 11.0));
	protectedCase.push_back(std::tuple<int, int>(-5.0, 11.0));
	protectedCase.push_back(std::tuple<int, int>(-7.0, 10.0));
	protectedCase.push_back(std::tuple<int, int>(-7.0, 9.0));
	protectedCase.push_back(std::tuple<int, int>(7.0, -11.0));
	_entities.push_back(
		new Box(glm::vec3(-7.0, 0, 9.0), _camera,
				rand() % 2 == 0 ? "DomeFossil" : "HelixFossil"));
	_entities.push_back(
		new Box(glm::vec3(-5.0, 0, 11.0), _camera,
				rand() % 2 == 0 ? "DomeFossil" : "HelixFossil"));

	std::vector<std::string> border;
	border.push_back("StrengthBoulder");
	std::vector<std::string> undestructibleBlock;
	undestructibleBlock.push_back("StrengthBoulder");
	std::vector<std::string> destructibleBlock;
	destructibleBlock.push_back("DomeFossil");
	destructibleBlock.push_back("HelixFossil");
	std::vector<std::string> enemies;
	enemies.push_back("Fuzzy");

	_createMap(8, 12, protectedCase, 10, 20, border, undestructibleBlock,
			   destructibleBlock, enemies);

	// Add entities that will be dropped
	_spawnableEntities.push_back(portal);

	// Force add of models (for entities that may be dropped randomly)
	_neededAssets.insert("Bomb");
	_neededAssets.insert("Sphere");
	_neededAssets.insert("Portal");
	_neededAssets.insert("KickPerk");
	_neededAssets.insert("DamagePerk");
	_neededAssets.insert("MaxBombPerk");
	_neededAssets.insert("RangePerk");
	_neededAssets.insert("SpeedPerk");

	// Dropped sounds
	_neededSounds.insert("portal_spawn");
}

void Bomberman::_mario(void) {
	_skybox = new Skybox("Default");
	WorldLocation dialogueLocation(glm::vec3(28.5, 15.8, 22.0),
								   glm::vec3(-32.0, 37.6, 0.0));
	WorldLocation gameplayLocation(glm::vec3(-16, 20.0, 11.0),
								   glm::vec3(-60.0, 0.0, 0.0));
	_camera = new Mario(dialogueLocation, gameplayLocation, 3.0f, this);
	_light = new Light(glm::vec2(-20.0, 8.0), glm::vec3(0.0f));
	_entities.push_back(new Player(glm::vec3(-19.0, 0.0, -5.0), glm::vec3(0.0f),
								   _save, _camera));

	// Portal to clear lvl
	std::vector<std::tuple<int, int>> protectedCase;
	protectedCase.push_back(std::tuple<int, int>(-19.0, -5.0));
	protectedCase.push_back(std::tuple<int, int>(-18.0, -5.0));
	protectedCase.push_back(std::tuple<int, int>(-19.0, -4.0));
	protectedCase.push_back(std::tuple<int, int>(-17.0, -5.0));
	protectedCase.push_back(std::tuple<int, int>(-19.0, -3.0));
	protectedCase.push_back(std::tuple<int, int>(17.0, 3.0));
	protectedCase.push_back(std::tuple<int, int>(19.0, 5.0));

	_entities.push_back(
		new Box(glm::vec3(-17.0, 0, -5.0), _camera, "RockPipe"));
	_entities.push_back(
		new Box(glm::vec3(-19.0, 0, -3.0), _camera, "RockPipe"));
	Entity *portal = new Portal(glm::vec3(0), _camera);
	_entities.push_back(
		new Box(glm::vec3(19, 0, 5), _camera, "RockPipe", 0, portal));

	protectedCase.push_back(std::tuple<int, int>(-20.0, -6.0));
	_entities.push_back(
		new Box(glm::vec3(-20.0, 0, -6.0), _camera, "WarpPipeCorner"));
	protectedCase.push_back(std::tuple<int, int>(-20.0, 6.0));
	_entities.push_back(
		new Box(glm::vec3(-20.0, 0, 6.0), _camera, "WarpPipeCorner"));
	_entities.back()->rotateY(90);
	protectedCase.push_back(std::tuple<int, int>(20.0, -6.0));
	_entities.push_back(
		new Box(glm::vec3(20.0, 0, -6.0), _camera, "WarpPipeCorner"));
	_entities.back()->rotateY(270);
	protectedCase.push_back(std::tuple<int, int>(20.0, 6.0));
	_entities.push_back(
		new Box(glm::vec3(20.0, 0, 6.0), _camera, "WarpPipeCorner"));
	_entities.back()->rotateY(180);

	for (int i = -19; i < 20; i++) {
		protectedCase.push_back(std::tuple<int, int>(i, -6.0));
		_entities.push_back(new Entity(
			glm::vec3(i, 0.0, -6.0), glm::vec3(0.0f),
			new Collider(Collider::Rectangle, LayerTag::WallLayer, 0.5, 0.5),
			"WarpPipe", "Wall", "Wall", _camera));
		_entities.back()->rotateY(90);
		protectedCase.push_back(std::tuple<int, int>(i, 6.0));
		_entities.push_back(new Entity(
			glm::vec3(i, 0.0, 6.0), glm::vec3(0.0f),
			new Collider(Collider::Rectangle, LayerTag::WallLayer, 0.5, 0.5),
			"WarpPipe", "Wall", "Wall", _camera));
		_entities.back()->rotateY(90);
	}

	_entities.push_back(new Entity(glm::vec3(0.0f), glm::vec3(0.0f), nullptr,
								   "CloudMap", "CloudMap", "CloudMap"));
	_entities.back()->rotateY(90);

	protectedCase.push_back(std::tuple<int, int>(1.0, 1.0));
	_entities.push_back(new EnemyBomber(glm::vec3(1, 0.0, 1), glm::vec3(0.0f),
										"EnemyBomber", _camera));
	protectedCase.push_back(std::tuple<int, int>(1.0, 3.0));
	_entities.push_back(new EnemyBomber(glm::vec3(1, 0.0, 3), glm::vec3(0.0f),
										"EnemyBomber", _camera));
	protectedCase.push_back(std::tuple<int, int>(3.0, 1.0));
	_entities.push_back(new EnemyBomber(glm::vec3(3, 0.0, 1), glm::vec3(0.0f),
										"EnemyBomber", _camera));
	protectedCase.push_back(std::tuple<int, int>(-1.0, -1.0));
	_entities.push_back(new EnemyBomber(glm::vec3(-1, 0.0, -1), glm::vec3(0.0f),
										"EnemyBomber", _camera));
	protectedCase.push_back(std::tuple<int, int>(-1.0, -3.0));
	_entities.push_back(new EnemyBomber(glm::vec3(-1, 0.0, -3), glm::vec3(0.0f),
										"EnemyBomber", _camera));
	protectedCase.push_back(std::tuple<int, int>(-3.0, -1.0));
	_entities.push_back(new EnemyBomber(glm::vec3(-3, 0.0, -1), glm::vec3(0.0f),
										"EnemyBomber", _camera));
	protectedCase.push_back(std::tuple<int, int>(3.0, 3.0));
	_entities.push_back(new EnemyBomber(glm::vec3(3, 0.0, 3), glm::vec3(0.0f),
										"EnemyBomber", _camera));
	protectedCase.push_back(std::tuple<int, int>(-3.0, -3.0));
	_entities.push_back(new EnemyBomber(glm::vec3(-3, 0.0, -3), glm::vec3(0.0f),
										"EnemyBomber", _camera));

	std::vector<std::string> border;
	border.push_back("WarpPipe");
	std::vector<std::string> undestructibleBlock;
	undestructibleBlock.push_back("Pipe");
	std::vector<std::string> destructibleBlock;
	destructibleBlock.push_back("RockPipe");
	std::vector<std::string> enemies;
	enemies.push_back("Fuzzy");

	_createMap(20, 6, protectedCase, 10, 100, border, undestructibleBlock,
			   destructibleBlock, enemies);

	// Force add of models (for entities that may be dropped randomly)
	_neededAssets.insert("Bomb");
	_neededAssets.insert("Sphere");
	_neededAssets.insert("Portal");
	_neededAssets.insert("KickPerk");
	_neededAssets.insert("DamagePerk");
	_neededAssets.insert("MaxBombPerk");
	_neededAssets.insert("RangePerk");
	_neededAssets.insert("SpeedPerk");

	// Dropped sounds
	_neededSounds.insert("portal_spawn");
}

void Bomberman::_space(void) {
	_skybox = new Skybox("CyanSpace");
	WorldLocation dialogueLocation(glm::vec3(18.0, 7.0, 26.3),
								   glm::vec3(-4.5, 24.3, 0.0));
	WorldLocation gameplayLocation(glm::vec3(-10, 20.0, 6.0),
								   glm::vec3(-60.0, 0.0, 0.0));
	_camera = new Space(dialogueLocation, gameplayLocation, 3.0f, this);
	_light = new Light(glm::vec2(-20.0, 8.0), glm::vec3(0.0f));
	_entities.push_back(new Entity(glm::vec3(0), glm::vec3(0.0f), nullptr,
								   "BigMeteor", "BigMeteor", "BigMeteor"));
	_entities.push_back(new Player(glm::vec3(-13.0, 0.0, -9.0), glm::vec3(0.0f),
								   _save, _camera));
	std::vector<std::tuple<int, int>> protectedCase;

	// Portal
	int randZone = rand() % 3;
	Entity *portal = new Portal(glm::vec3(0), _camera);
	_entities.push_back(new Box(glm::vec3(13, 0, -9), _camera,
								"DestructibleMeteor", 0,
								(randZone == 0) ? portal : nullptr));
	protectedCase.push_back(std::tuple<int, int>(13, -9));
	_entities.push_back(new Box(glm::vec3(-13, 0, 9), _camera,
								"DestructibleMeteor", 0,
								(randZone == 1) ? portal : nullptr));
	protectedCase.push_back(std::tuple<int, int>(-13, 9));
	_entities.push_back(new Box(glm::vec3(13, 0, 9), _camera,
								"DestructibleMeteor", 0,
								(randZone == 2) ? portal : nullptr));
	protectedCase.push_back(std::tuple<int, int>(13, 9));

	// Enemy
	int xCenter = 7;
	int zCenter = -5;
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter - 4),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter - 4));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter + 4),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter + 4));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter + 4, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter + 4, zCenter));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter - 4, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter - 4, zCenter));

	xCenter = 7;
	zCenter = 5;
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter - 4),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter - 4));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter + 4),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter + 4));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter + 4, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter + 4, zCenter));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter - 4, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter - 4, zCenter));

	xCenter = -7;
	zCenter = 5;
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter - 4),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter - 4));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter, 0.0, zCenter + 4),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter, zCenter + 4));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter + 4, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter + 4, zCenter));
	_entities.push_back(new EnemyMelee(glm::vec3(xCenter - 4, 0.0, zCenter),
									   glm::vec3(0.0f), "RedGhost", _camera));
	protectedCase.push_back(std::tuple<int, int>(xCenter - 4, zCenter));

	// Border
	for (int tmpY = -9; tmpY < 10; tmpY += 2) {
		_entities.push_back(
			new Box(glm::vec3(0, 0, tmpY), _camera, "DestructibleMeteor", 66));
		protectedCase.push_back(std::tuple<int, int>(0, tmpY));
	}
	for (int tmpX = -13; tmpX <= 13; tmpX += 2) {
		_entities.push_back(
			new Box(glm::vec3(tmpX, 0, 0), _camera, "DestructibleMeteor", 66));
		protectedCase.push_back(std::tuple<int, int>(tmpX, 0));
	}

	// Safe start zone
	protectedCase.push_back(std::tuple<int, int>(-13.0, -9.0));
	protectedCase.push_back(std::tuple<int, int>(-13.0, -8.0));
	protectedCase.push_back(std::tuple<int, int>(-12.0, -9.0));

	std::vector<std::string> border;
	border.push_back("Meteor");
	std::vector<std::string> undestructibleBlock;
	undestructibleBlock.push_back("Meteor");
	std::vector<std::string> destructibleBlock;
	destructibleBlock.push_back("DestructibleMeteor");
	std::vector<std::string> enemies;
	enemies.push_back("Fuzzy");
	_createMap(14, 10, protectedCase, 10, 1000, border, undestructibleBlock,
			   destructibleBlock, enemies);

	// Assets
	_neededAssets.insert("Bomb");
	_neededAssets.insert("Sphere");
	_neededAssets.insert("Portal");
	_neededAssets.insert("KickPerk");
	_neededAssets.insert("DamagePerk");
	_neededAssets.insert("MaxBombPerk");
	_neededAssets.insert("RangePerk");
	_neededAssets.insert("SpeedPerk");

	// Dropped sounds
	_neededSounds.insert("portal_spawn");
}

void Bomberman::_credits(void) {
	_skybox = new Skybox("BlueSpace");
	WorldLocation startLocation(glm::vec3(-14.5, 20.0, -3.0),
								glm::vec3(-60.0, 0.0, 0.0));
	_camera = new Credits(startLocation, this);
	_light = new Light(glm::vec2(-10.0, -10.0), glm::vec3(0.0f), 10.0f);
}

void Bomberman::_initScenes(void) {
	_scenesNames.push_back(_startLevelName);
	_scenesMap[_scenesNames.back()] = &Bomberman::_mainMenu;

	_scenesNames.push_back("Forest");
	_scenesMap[_scenesNames.back()] = &Bomberman::_forest;
	_scenesNames.push_back("Pokemon");
	_scenesMap[_scenesNames.back()] = &Bomberman::_pokemon;
	_scenesNames.push_back("Mario");
	_scenesMap[_scenesNames.back()] = &Bomberman::_mario;
	_scenesNames.push_back("Space");
	_scenesMap[_scenesNames.back()] = &Bomberman::_space;
	_scenesNames.push_back("Credits");
	_scenesMap[_scenesNames.back()] = &Bomberman::_credits;
}

void Bomberman::_createMap(int width, int height,
						   std::vector<std::tuple<int, int>> &protectedCase,
						   size_t boxRate, size_t monsterRate,
						   std::vector<std::string> border,
						   std::vector<std::string> undestructibleBlock,
						   std::vector<std::string> destructibleBlock,
						   std::vector<std::string> enemies) {
	for (int x = -width; x <= width; x++) {
		for (int z = -height; z <= height; z++) {
			bool canPutBlocks = true;
			for (const auto &t : protectedCase) {
				if (std::get<1>(t) == z && std::get<0>(t) == x) {
					canPutBlocks = false;
				}
			}
			if (canPutBlocks && (abs(x) == width || abs(z) == height)) {
				_entities.push_back(new Entity(
					glm::vec3(x, 0.0, z), glm::vec3(0.0f),
					new Collider(Collider::Rectangle, LayerTag::WallLayer, 0.45,
								 0.45),
					border[rand() % border.size()], "Wall", "Wall", _camera));
			} else if (canPutBlocks && x % 2 == 0 && z % 2 == 0 &&
					   width % 2 == 0 && height % 2 == 0) {
				_entities.push_back(new Entity(
					glm::vec3(x, 0.0, z), glm::vec3(0.0f),
					new Collider(Collider::Rectangle, LayerTag::WallLayer, 0.5,
								 0.5),
					undestructibleBlock[rand() % undestructibleBlock.size()],
					"Wall", "Wall", _camera));

			} else if (canPutBlocks && x % 2 != 0 && z % 2 != 0 &&
					   width % 2 != 0 && height % 2 != 0) {
				_entities.push_back(new Entity(
					glm::vec3(x, 0.0, z), glm::vec3(0.0f),
					new Collider(Collider::Rectangle, LayerTag::WallLayer, 0.5,
								 0.5),
					undestructibleBlock[rand() % undestructibleBlock.size()],
					"Wall", "Wall", _camera));

			} else {
				if (canPutBlocks && rand() % boxRate == 0) {
					_entities.push_back(new Box(
						glm::vec3(x, 0, z), _camera,
						destructibleBlock[rand() % destructibleBlock.size()]));
				} else if (canPutBlocks && rand() % monsterRate == 0) {
					_entities.push_back(new EnemyBasic(
						glm::vec3(x, 0.0, z), glm::vec3(0.0f),
						enemies[rand() % enemies.size()], _camera));
				}
			}
		}
	}
}

size_t Bomberman::getWindowWidth() {
	return std::get<1>(Save::RESOLUTIONS[_save.resolutionsIdx]);
}
size_t Bomberman::getWindowHeight() {
	return std::get<2>(Save::RESOLUTIONS[_save.resolutionsIdx]);
}

int Bomberman::getFirstSceneIdx(void) const {
	return getSceneIndexByName(_startLevelName);
}
bool Bomberman::isFullScreen() {
	return std::get<1>(Save::FULL_SCREEN[_save.isFullScreen]);
}
