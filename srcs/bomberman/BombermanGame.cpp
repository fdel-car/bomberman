#include "bomberman/BombermanGame.hpp"
#include "bomberman/cams/ForestCam.hpp"
#include "bomberman/cams/MainMenuCam.hpp"
#include "bomberman/entities/Enemy.hpp"
#include "bomberman/entities/Player.hpp"

#include <fstream>
#include <iomanip>
#include "json/json.hpp"  // https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp

extern std::string _assetsDir;

void to_json(nlohmann::json& j, const Save& p) {
	j = nlohmann::json{{"upKey", p.upKey},
					   {"leftKey", p.leftKey},
					   {"downKey", p.downKey},
					   {"rightKey", p.rightKey}};
}

void from_json(const nlohmann::json& j, Save& p) {
	j.at("upKey").get_to(p.upKey);
	j.at("leftKey").get_to(p.leftKey);
	j.at("downKey").get_to(p.downKey);
	j.at("rightKey").get_to(p.rightKey);
}

void Save::resetSettings(void) {
	upKey = "W";
	leftKey = "A";
	downKey = "S";
	rightKey = "D";
	bombKey = "SPACE";
}

void Save::resetLevel(void) { level = 0; }

void Save::resetAll(void) {
	resetSettings();
	resetLevel();
}

BombermanGame::BombermanGame(void) : AGame() {
	for (float size = 12.0f; size <= 48.0f; size += 1.0f)
		_neededFonts.push_back(std::tuple<float, std::string, std::string>(
			size, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_initScenes();
	getSave();
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
	_camera = new ForestCam(glm::vec3(0.0f, 28.0f, 16.8f),
							glm::vec3(-60.0f, 0.0f, 0.0f));
	// _entities.push_back(new Entity(glm::vec3(0.0, 0.0, 0.0),
	// 							   glm::vec3(0.0, 0.0, 0.0f), nullptr,
	// 							   "Island"));
	_entities.push_back(new Player(glm::vec3(-7.0f, 0.5f, -7.0f),
								   glm::vec3(0.0f, 0.0f, 0.0f), _camera));
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

void BombermanGame::getSave(void) {
	std::ifstream rFile(BombermanGame::SAVE_FILE);
	if (rFile.is_open()) {
		std::string line;
		std::string allLines = "";

		while (getline(rFile, line)) {
			allLines += line;
		}
		rFile.close();

		nlohmann::json parsedJson = nlohmann::json::parse(allLines.c_str());

		try {
			BombermanGame::save = parsedJson;
		} catch (std::exception e) {
			BombermanGame::initNewSave();
		}
	} else
		BombermanGame::initNewSave();
}

void BombermanGame::initNewSave(void) {
	save.resetAll();
	doSave();
}

void BombermanGame::doSave(void) {
	std::ofstream myfile(SAVE_FILE);
	if (myfile.is_open()) {
		nlohmann::json myJson = save;
		myfile << std::setw(4) << myJson << std::endl;
		myfile.close();
	}
}

const std::string getSaveFilePath() {
	std::string path = __FILE__;
	path.erase(path.begin() + path.rfind("/BombermanGame.cpp") + 1, path.end());
	path += "save.txt";
	return path;
}
const std::string BombermanGame::SAVE_FILE = getSaveFilePath();

Save BombermanGame::save = Save();