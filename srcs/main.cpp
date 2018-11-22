#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"

std::string _assetsDir;
std::string _srcsDir;

int main(void) {
	_srcsDir = __FILE__;
	_srcsDir.erase(_srcsDir.begin() + _srcsDir.rfind("/srcs/") + 1,
				   _srcsDir.end());
	_assetsDir = _srcsDir + "assets/";
	_srcsDir += "srcs/";
	try {
		/* Initialize random seed: */
		srand(time(NULL));
		AGame *myGame = new Bomberman();
		GameEngine gameEngine(myGame);
		gameEngine.run();
		delete myGame;
	} catch (const std::runtime_error &err) {
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
