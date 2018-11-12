#include "GameLogic.hpp"

int main(void)
{
    int ret = 0;

    try
    {
        /* initialize random seed: */
        srand(time(NULL));
        GameLogic mainGame;
		// std::cout << "test" << std::endl;
        ret = mainGame.run();
    }
    catch (...)
    {
        std::cerr << "Unkown error occurred when running game" << std::endl;
        return EXIT_FAILURE;
    }
    return ret;
}
