#include "scenes/BomberManGame.hpp"
#include "scenes/MainMenu.hpp"
#include "scenes/Level01.hpp"

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

	gameScenes = std::vector<AGameScene *>();
	gameScenes.push_back(new MainMenu());
	gameScenes.push_back(new Level01());
}

BomberManGame::~BomberManGame(void) { }
