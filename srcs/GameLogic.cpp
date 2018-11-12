#include "GameLogic.hpp"
#include "GameRenderer.hpp"

#include "Player.hpp" // TODO: this should be done by a "game initializer" and not by the game engine!

GameLogic::GameLogic()
{
	canRun = false;
	running = false;

	// Map size parsing
	mapW = MAP_SIZE;
	mapH = MAP_SIZE;
	int maxSizeW = (WINDOW_W - 2 * WINDOW_MIN_X_OFFSET) / mapW;
	int maxSizeH = (WINDOW_H - 2 * WINDOW_MIN_Y_OFFSET) / mapH;
	squareSize = (maxSizeH < maxSizeW) ? maxSizeH : maxSizeW;
	xOffset = (WINDOW_W - squareSize * mapW) / 2;
	yOffset = (WINDOW_H - squareSize * mapH) / 2;

	// Create interface class
	// std::cout << "ALLER1" << std::endl;
	graphicLib = new GameRenderer(this);
	// std::cout << "ALLER2" << std::endl;
	if (!graphicLib)
	{
		std::cerr << "GameRenderer couldn't load !" << std::endl;
		return;
	}
	// Create audio manager
	// std::cout << "ALLER3" << std::endl;
	audioManager = new AudioManager();

	// std::cout << "ALLER4" << std::endl;
	if (!audioManager)
	{
		std::cerr << "AudioManager couldn't load !" << std::endl;
		return;
	}
	// std::cout << "ALLER5" << std::endl;

	// TODO: this should be done by a "game initializer" and not by the game engine!
	entities.push_back(new Player());

	// Everything good
	canRun = true;
}

GameLogic::GameLogic(GameLogic const &src)
{
	*this = src;
	return;
}

GameLogic::~GameLogic(void)
{
	for (auto entity : entities)
	{
		delete entity;
	}
	delete (audioManager);
	delete (graphicLib);
	return;
}

int GameLogic::getSquareSize(void)
{
	return squareSize;
}

int GameLogic::getXOffset(void)
{
	return xOffset;
}

int GameLogic::getYOffset(void)
{
	return yOffset;
}

int GameLogic::getMapW(void)
{
	return mapW;
}

int GameLogic::getMapH(void)
{
	return mapH;
}

int GameLogic::getPlayerDirection(void)
{
	return playerDirection;
}

bool GameLogic::getIsPlayerAlive(void)
{
	return isPlayerAlive;
}

std::tuple<int, int> &GameLogic::getPlayerPos(void)
{
	return playerPos;
}

GameLogic &GameLogic::operator=(GameLogic const &rhs)
{
	this->canRun = rhs.canRun;
	return *this;
}

void GameLogic::printUsage(void)
{
	std::cerr << "Usage: ./bomberman" << std::endl;
}

void GameLogic::changeLibraryRequest(std::string keyCode)
{
	int requestedIndex = std::stoi(keyCode);

	std::cout << "Change index of library to: " << requestedIndex << std::endl;
	if (requestedIndex >= 0 && requestedIndex <= 0)
	{
		dlIndex = requestedIndex;
	}
}

void GameLogic::updateGameState(void)
{
	// Get all pool events in library
	graphicLib->getUserInput();

	// Check if we want to close window, in this case no need to do further calculations
	if (dlIndex == 0)
	{
		running = false;
	}

	if (isPlayerAlive && running)
	{
		if (playerDirectionRequested > 0)
		{
			playerDirection = playerDirectionRequested;
			playerDirectionRequested = -1;

			if (playerCanMove())
			{
				movePlayer(playerPos, playerDirection);
			}

			//player actual moving
			if (!isPlayerAlive)
			{
				std::cerr << "Game Over! (Press 'R' to restart)" << std::endl;
				audioManager->playDeathSound();
			}
		}
	}
}

int GameLogic::renderGame(void)
{
	if (dlIndex < 0 || dlIndex > 1)
	{
		std::cerr << "Wrong number given.." << std::endl;
		return EXIT_FAILURE;
	}

	// Draw window with game infos
	if (graphicLib && !hasShownDeath)
	{
		graphicLib->refreshWindow();
		if (!isPlayerAlive)
			hasShownDeath = true;
	}

	return EXIT_SUCCESS;
}

void GameLogic::initPlayer(void)
{
	isPlayerAlive = true;
	hasShownDeath = false;

	playerPos = std::tuple<int, int>();

	playerPos = std::make_tuple(mapW / 2, mapH / 2);
	playerDirection = (mapW > mapH) ? LEFT : DOWN;
	playerDirectionRequested = -1;
}

bool GameLogic::playerCanMove(void)
{
	// Check for player
	int headX = std::get<0>(playerPos);
	int headY = std::get<1>(playerPos);
	if (playerDirection == UP)
		headY -= 1;
	else if (playerDirection == DOWN)
		headY += 1;
	else if (playerDirection == LEFT)
		headX -= 1;
	else if (playerDirection == RIGHT)
		headX += 1;

	return true;
}

void GameLogic::movePlayer(std::tuple<int, int> &playerPos, int &playerDir)
{
	// Advance based on direction
	if (playerDir == UP)
		std::get<1>(playerPos) = std::get<1>(playerPos) - 1;
	else if (playerDir == DOWN)
		std::get<1>(playerPos) = std::get<1>(playerPos) + 1;
	else if (playerDir == LEFT)
		std::get<0>(playerPos) = std::get<0>(playerPos) - 1;
	else if (playerDir == RIGHT)
		std::get<0>(playerPos) = std::get<0>(playerPos) + 1;
}

void GameLogic::changeDirectionTo(int &playerDirection, int &playerDirectionRequested, int newDir)
{
	playerDirectionRequested = newDir;
	if ((newDir == UP || newDir == DOWN) && (playerDirection == LEFT || playerDirection == RIGHT))
	{
		playerDirectionRequested = newDir;
	}
	else if ((newDir == LEFT || newDir == RIGHT) && (playerDirection == UP || playerDirection == DOWN))
	{
		playerDirectionRequested = newDir;
	}
}

int GameLogic::run(void)
{
	// std::cout << "COUCOU1" << std::endl;
	if (!canRun)
		return EXIT_FAILURE;
	int guiRet;

	// init vars
	running = true;
	restartRequest = false;
	dlIndex = 1;
	dlPastIndex = -1;
	timer = time(NULL);

	initPlayer();

	audioManager->playStartSound();

	// Start game loop
	while (running)
	{
		// Get delta time in order to synch entities positions
		pastFrameLength = difftime(timer, time(NULL));
		timer = time(NULL);

		// Update inputs
		// std::cout << "COUCOU2" << std::endl;
		graphicLib->getUserInput();

		// TODO: Update game engine statuses (ex. when to quit)
		if (running)
		{
			running = !keyboardMap["ESCAPE"];
		}

		// Update game entities states
		// for (auto entity : entities)
		// {
		// 	entity->Update();
		// }

		// TODO: set position of entities back to prev frame when they collide

		updateGameState();

		guiRet = renderGame();
		if (guiRet != EXIT_SUCCESS || !running)
			break;
	}
	if (restartRequest)
	{
		// std::cout << "Starting new game!" << std::endl;
		return run();
	}
	return guiRet;
}

void GameLogic::buttonStateChanged(std::string buttonName, bool isPressed)
{
	// std::map<std::string, bool> mapKey;
	// mapKey[buttonName] = isPressed;
	// std::string key = !button ? "NULL" : std::string(button); // GLFW sends NULL pointer for Escape key..
	// std::cout << "key '" << buttonName << "' new status: " << isPressed << std::endl;
	if (keyboardMap.find(buttonName) == keyboardMap.end())
	{
		std::runtime_error("Unkown Mapping for '" + buttonName + "' !");
	}
	keyboardMap[buttonName] = isPressed;
}

static std::map<std::string, bool> generateKeyboardMap()
{ // static here is "internal linkage"
	std::map<std::string, bool> m = std::map<std::string, bool>();
	m["UNKNOWN"] = false;
	m["SPACE"] = false;
	m["APOSTROPHE"] = false;
	m["COMMA"] = false;
	m["MINUS"] = false;
	m["PERIOD"] = false;
	m["SLASH"] = false;
	m["0"] = false;
	m["1"] = false;
	m["2"] = false;
	m["3"] = false;
	m["4"] = false;
	m["5"] = false;
	m["6"] = false;
	m["7"] = false;
	m["8"] = false;
	m["9"] = false;
	m[";"] = false;
	m["EQUAL"] = false;
	m["A"] = false;
	m["B"] = false;
	m["C"] = false;
	m["D"] = false;
	m["E"] = false;
	m["F"] = false;
	m["G"] = false;
	m["H"] = false;
	m["I"] = false;
	m["L"] = false;
	m["K"] = false;
	m["L"] = false;
	m["M"] = false;
	m["N"] = false;
	m["O"] = false;
	m["P"] = false;
	m["Q"] = false;
	m["R"] = false;
	m["S"] = false;
	m["T"] = false;
	m["U"] = false;
	m["V"] = false;
	m["W"] = false;
	m["X"] = false;
	m["Y"] = false;
	m["Z"] = false;
	m["["] = false;
	m["\\"] = false;
	m["]"] = false;
	m["`"] = false;
	m["WORLD_1"] = false;
	m["WORLD_2"] = false;
	m["ESCAPE"] = false;
	m["ENTER"] = false;
	m["TAB"] = false;
	m["BACKSPACE"] = false;
	m["INSERT"] = false;
	m["DELETE"] = false;
	m["RIGHT"] = false;
	m["LEFT"] = false;
	m["DOWN"] = false;
	m["UP"] = false;
	m["PAGE_UP"] = false;
	m["PAGE_DOWN"] = false;
	m["HOME"] = false;
	m["END"] = false;
	m["CAPS_LOCK"] = false;
	m["SCROLL_LOCK"] = false;
	m["NUM_LOCK"] = false;
	m["PRINT_SCREEN"] = false;
	m["PAUSE"] = false;
	m["F1"] = false;
	m["F2"] = false;
	m["F3"] = false;
	m["F4"] = false;
	m["F5"] = false;
	m["F6"] = false;
	m["F7"] = false;
	m["F8"] = false;
	m["F9"] = false;
	m["F10"] = false;
	m["F11"] = false;
	m["F12"] = false;
	m["F13"] = false;
	m["F14"] = false;
	m["F15"] = false;
	m["F16"] = false;
	m["F17"] = false;
	m["F18"] = false;
	m["F19"] = false;
	m["F20"] = false;
	m["F21"] = false;
	m["F22"] = false;
	m["F23"] = false;
	m["F24"] = false;
	m["F25"] = false;
	m["KP_0"] = false;
	m["KP_1"] = false;
	m["KP_2"] = false;
	m["KP_3"] = false;
	m["KP_4"] = false;
	m["KP_5"] = false;
	m["KP_6"] = false;
	m["KP_7"] = false;
	m["KP_8"] = false;
	m["KP_9"] = false;
	m["KP_DECIMAL"] = false;
	m["KP_DIVIDE"] = false;
	m["KP_MULTIPLY"] = false;
	m["KP_SUBTRACT"] = false;
	m["KP_ADD"] = false;
	m["KP_ENTER"] = false;
	m["KP_EQUAL"] = false;
	m["LEFT_SHIFT"] = false;
	m["LEFT_CONTROL"] = false;
	m["LEFT_ALT"] = false;
	m["LEFT_SUPER"] = false;
	m["RIGHT_SHIFT"] = false;
	m["RIGHT_CONTROL"] = false;
	m["RIGHT_ALT"] = false;
	m["RIGHT_SUPER"] = false;
	m["MENU"] = false;

	return m;
}
std::map<std::string, bool> GameLogic::keyboardMap = generateKeyboardMap();
