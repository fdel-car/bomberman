#include "AGame.hpp"
#include "GameEngine.hpp"
#include "scenes/BombermanGame.hpp"

std::string _assetsDir;

int main(void) {
	_assetsDir = __FILE__;
	_assetsDir.erase(_assetsDir.begin() + _assetsDir.rfind("/srcs/") + 1,
					 _assetsDir.end());
	_assetsDir += "assets/";
	try {
		/* Initialize random seed: */
		srand(time(NULL));
		AGame *myGame = new BombermanGame();
		GameEngine gameEngine(myGame);
		gameEngine.run();
		delete myGame;
	} catch (const std::runtime_error &err) {
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
