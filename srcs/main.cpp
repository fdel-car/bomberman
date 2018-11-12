#include "GameEngine.hpp"
#include "LevelOne.hpp"

const char *getFileContent(std::string const &filename) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in) {
		std::string str;
		in.seekg(0, std::ios::end);
		str.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&str[0], str.size());
		in.close();
		return (str.c_str());
	}
	throw(std::runtime_error("Could not read the file " +
							 static_cast<std::string>(filename) + "."));
}

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
