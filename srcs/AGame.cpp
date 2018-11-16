#include "AGame.hpp"
#include "scenes/MainMenu.hpp"
#include "scenes/Level01.hpp"

AGame::AGame(void) { }

AGame::~AGame(void) {
	for (auto scene : gameScenes) {
		delete scene;
	}
}

std::vector<AGameScene *> & AGame::getGameScenes() {
	return gameScenes;
}

std::vector<std::tuple<float, std::string, std::string>> & AGame::getNeededFont() {
	return vNeededFont;
}
