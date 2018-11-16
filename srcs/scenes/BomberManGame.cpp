#include "scenes/BomberManGame.hpp"
#include "scenes/MainMenuCam.hpp"
#include "scenes/Level01Cam.hpp"
#include "Player.hpp"

BomberManGame::BomberManGame(void) {
	std::string path = __FILE__;
	for (size_t i = path.size() - 1; i > 0; i--) {
		if (path[i] == '/') {
			path.erase(path.begin() + i + 1, path.end());
			break ;
		}
	}
	vNeededFont.push_back(std::tuple<float, std::string, std::string> (48.0f, (path + "../../assets/assetsGUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string> (42.0f, (path + "../../assets/assetsGUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string> (24.0f, (path + "../../assets/assetsGUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string> (22.0f, (path + "../../assets/assetsGUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string> (20.0f, (path + "../../assets/assetsGUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string> (18.0f, (path + "../../assets/assetsGUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
	vNeededFont.push_back(std::tuple<float, std::string, std::string> (14.0f, (path + "../../assets/assetsGUI/fonts/BOMBERMA.TTF"), "BOMBERMA"));
}

BomberManGame::~BomberManGame(void) { }


bool BomberManGame::loadScene(size_t sceneIdx) {
	unload();
	if (sceneIdx == 0) {
		_camera = new MainMenuCam(glm::vec3(0.0, 10.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

		_entities.push_back(
			new Player(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 45.0f)));
	}
	else if (sceneIdx == 1) {
		_camera = new Level01Cam(glm::vec3(0.0, 10.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

		_entities.push_back(
			new Player(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 45.0f)));
		_entities.push_back(
			new Entity(glm::vec3(2.0, 0.0, 2.0), glm::vec3(0.0, 0.0, 45.0f),
					   new Collider(Collider::Rectangle, 1.0f, 1.0f), "Cube"));
	}
	else
		return false;
	return true;
}
