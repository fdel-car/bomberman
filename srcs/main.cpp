#include "GameEngine.hpp"
#include "AGame.hpp"
#include "scenes/BomberManGame.hpp"

int main(void) {
	try {
		/* Initialize random seed: */
		srand(time(NULL));
		AGame * myGame = new BomberManGame();
		GameEngine gameEngine(myGame);
		gameEngine.run();
	} catch (const std::runtime_error &err) {
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
