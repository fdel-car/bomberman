#include "GameEngine.hpp"

int main(void) {
	int ret = 0;
	try {
		/* Initialize random seed: */
		srand(time(NULL));
		GameEngine mainGame;
		ret = mainGame.run();
	} catch (const std::runtime_error &err) {
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return ret;
}