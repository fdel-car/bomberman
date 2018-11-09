#include "GameLogic.hpp"

int main(void)
{
	int ret = 0;
	try
	{
		/* Initialize random seed: */
		srand(time(NULL));
		GameLogic mainGame;
		ret = mainGame.run();
	}
	catch (const std::runtime_error &err)
	{
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	return ret;
}