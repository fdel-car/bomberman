#include "scenes/BombermanGame.hpp"
#include "Player.hpp"
#include "scenes/Level01Cam.hpp"
#include "scenes/MainMenuCam.hpp"

extern std::string _assetsDir;

BombermanGame::BombermanGame(void) : AGame() {
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		48.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		47.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		46.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		45.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		44.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		43.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		42.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		41.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		40.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		30.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		29.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		28.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		27.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		26.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		25.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		24.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		23.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		22.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		21.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		20.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		19.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		18.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		17.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		16.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		15.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string>(
		14.0f, (_assetsDir + "GUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
}

BombermanGame::~BombermanGame(void) {}

bool BombermanGame::loadScene(size_t sceneIdx) {
	unload();
	if (sceneIdx == 0) {
		_camera = new MainMenuCam(glm::vec3(0.0, 10.0, 0.0),
								  glm::vec3(0.0, 0.0, 0.0));
		_entities.push_back(
			new Player(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 45.0f)));
	} else if (sceneIdx == 1) {
		_camera = new Level01Cam(glm::vec3(0.0, 10.0, 10.0),
								 glm::vec3(-45.0, 0.0, 0.0));

		_entities.push_back(
			new Player(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0f)));
		_entities.push_back(
			new Entity(glm::vec3(3.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0f),
					   new Collider(Collider::Rectangle, 1.0f, 1.0f), "Cube"));
	} else
		return false;
	return true;
}
