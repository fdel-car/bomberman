#include "scenes/BombermanGame.hpp"
#include "scenes/cams/ForestCam.hpp"
#include "scenes/cams/MainMenuCam.hpp"
#include "scenes/entities/Player.hpp"

#include <fstream>
#include <iomanip>
#include "json/json.hpp"  // https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp

extern std::string _assetsDir;

void to_json(nlohmann::json& j, const Save& p) {
	j = nlohmann::json{
		{"upKey", p.upKey}  // ,{"toto" = 42}
	};
}

void from_json(const nlohmann::json& j, Save& p) {
	j.at("upKey").get_to(p.upKey);
	// j.at("downKey").get_to(p.downKey);
	// j.at("age").get_to(p.age);
}

BombermanGame::BombermanGame(void) : AGame() {
	for (float size = 12.0f; size <= 48.0f; size += 1.0f)
		_neededFonts.push_back(std::tuple<float, std::string, std::string>(
			size, (_assetsDir + "GUI/fonts/BOMBERMAN.ttf"), "BOMBERMAN"));
	_initScenes();

	// create a JSON object
	// nlohmann::json j = {{"pi", 3.141},
	// 					{"happy", true},
	// 					{"name", "Niels"},
	// 					{"nothing", nullptr},
	// 					{"answer", {{"everything", 42}}},
	// 					{"list", {1, 0, 2}},
	// 					{"object", {{"currency", "USD"}, {"value", 42.99}}}};

	// // add new values
	// j["new"]["key"]["value"] = {"another", "list"};

	// // count elements
	// auto s = j.size();
	// j["size"] = s;

	// // pretty print with indent of 4 spaces
	// std::cout << std::setw(4) << j << '\n';

	// Save save;

	// nlohmann::json jTest = save;
	// std::cout << "Should look like:\n\t" << jTest << std::endl;

	// std::cout << "Saving to file.." << std::endl;
	// std::ofstream myfile("example.txt");
	// if (myfile.is_open()) {
	// 	myfile << std::setw(4) << jTest << std::endl;
	// 	myfile.close();
	// }

	// nlohmann::json j_complete = nlohmann::json::parse(char *txt);

	std::cout << "Reading from file.." << std::endl;
	std::string line;
	std::string allLines = "";
	std::ifstream rFile("example.txt");
	if (rFile.is_open()) {
		while (getline(rFile, line)) {
			std::cout << line << '\n';
			allLines += line;
		}
		rFile.close();
	}
	nlohmann::json parsedJson = nlohmann::json::parse(allLines.c_str());

	try {
		Save saveT = parsedJson;
		std::cout << "Save val: " << saveT.upKey << std::endl;
	} catch (std::exception e) {
		std::cout << "--------------------- Could not convert json to struct"
				  << std::endl;
	}
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
	_camera =
		new ForestCam(glm::vec3(0.0, 10.0, 3.0), glm::vec3(-75.0, 0.0, 0.0));
	_entities.push_back(
		new Player(glm::vec3(2.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 0.0f)));
	_entities.push_back(
		new Entity(glm::vec3(-2.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0f),
				   new Collider(Collider::Rectangle, 1.0f, 1.0f), "Cube"));
}

void BombermanGame::_initScenes(void) {
	_scenesNames.push_back("MainMenu");
	_scenesMap[_scenesNames.back()] = &BombermanGame::_mainMenu;
	_scenesNames.push_back("Forest");
	_scenesMap[_scenesNames.back()] = &BombermanGame::_forest;
}