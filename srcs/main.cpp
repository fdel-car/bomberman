#include "GameEngine.hpp"
#include "LevelOne.hpp"

int main(void) {
	int ret = 0;
	try {
		/* Initialize random seed: */
		srand(time(NULL));
		std::vector<AGameScene *> gameScenes = std::vector<AGameScene *>();
		gameScenes.push_back(new LevelOne());
		GameEngine gameEngine(gameScenes);
		ret = gameEngine.run();

		for (auto scene : gameScenes) {
			delete scene;
		}
	} catch (const std::runtime_error &err) {
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return ret;
}