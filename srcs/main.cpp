#include "GameEngine.hpp"
#include "scenes/Factory.hpp"

int main(void) {
	try {
		/* Initialize random seed: */
		srand(time(NULL));
		std::vector<AGameScene *> gameScenes = std::vector<AGameScene *>();
		gameScenes.push_back(new Factory());
		GameEngine gameEngine(gameScenes);
		gameEngine.run();
		for (auto scene : gameScenes) {
			delete scene;
		}
	} catch (const std::runtime_error &err) {
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}