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
	graphicLib = new GameRenderer(this);
	// Create audio manager
	audioManager = new AudioManager();

	// TODO: this should be done by a "game initializer" and not by the game engine!
	entities.push_back(new Player());

	// Everything good
	canRun = true;
}

GameLogic::GameLogic(GameLogic const &src)
{
	*this = src;
}

GameLogic::~GameLogic(void)
{
	for (auto entity : entities)
	{
		delete entity;
	}
	delete (audioManager);
	delete (graphicLib);
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

	// std::cout << "Change index of library to: " << requestedIndex << std::endl;
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
		std::cout << "Starting new game!" << std::endl;
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
	std::map<std::string, bool> map = std::map<std::string, bool>();
	map["UNKNOWN"] = false;
	map["SPACE"] = false;
	map["APOSTROPHE"] = false;
	map["COMMA"] = false;
	map["MINUS"] = false;
	map["PERIOD"] = false;
	map["SLASH"] = false;
	map["0"] = false;
	map["1"] = false;
	map["2"] = false;
	map["3"] = false;
	map["4"] = false;
	map["5"] = false;
	map["6"] = false;
	map["7"] = false;
	map["8"] = false;
	map["9"] = false;
	map[";"] = false;
	map["EQUAL"] = false;
	map["A"] = false;
	map["B"] = false;
	map["C"] = false;
	map["D"] = false;
	map["E"] = false;
	map["F"] = false;
	map["G"] = false;
	map["H"] = false;
	map["I"] = false;
	map["L"] = false;
	map["K"] = false;
	map["L"] = false;
	map["M"] = false;
	map["N"] = false;
	map["O"] = false;
	map["P"] = false;
	map["Q"] = false;
	map["R"] = false;
	map["S"] = false;
	map["T"] = false;
	map["U"] = false;
	map["V"] = false;
	map["W"] = false;
	map["X"] = false;
	map["Y"] = false;
	map["Z"] = false;
	map["["] = false;
	map["\\"] = false;
	map["]"] = false;
	map["`"] = false;
	map["WORLD_1"] = false;
	map["WORLD_2"] = false;
	map["ESCAPE"] = false;
	map["ENTER"] = false;
	map["TAB"] = false;
	map["BACKSPACE"] = false;
	map["INSERT"] = false;
	map["DELETE"] = false;
	map["RIGHT"] = false;
	map["LEFT"] = false;
	map["DOWN"] = false;
	map["UP"] = false;
	map["PAGE_UP"] = false;
	map["PAGE_DOWN"] = false;
	map["HOME"] = false;
	map["END"] = false;
	map["CAPS_LOCK"] = false;
	map["SCROLL_LOCK"] = false;
	map["NUM_LOCK"] = false;
	map["PRINT_SCREEN"] = false;
	map["PAUSE"] = false;
	map["F1"] = false;
	map["F2"] = false;
	map["F3"] = false;
	map["F4"] = false;
	map["F5"] = false;
	map["F6"] = false;
	map["F7"] = false;
	map["F8"] = false;
	map["F9"] = false;
	map["F10"] = false;
	map["F11"] = false;
	map["F12"] = false;
	map["F13"] = false;
	map["F14"] = false;
	map["F15"] = false;
	map["F16"] = false;
	map["F17"] = false;
	map["F18"] = false;
	map["F19"] = false;
	map["F20"] = false;
	map["F21"] = false;
	map["F22"] = false;
	map["F23"] = false;
	map["F24"] = false;
	map["F25"] = false;
	map["KP_0"] = false;
	map["KP_1"] = false;
	map["KP_2"] = false;
	map["KP_3"] = false;
	map["KP_4"] = false;
	map["KP_5"] = false;
	map["KP_6"] = false;
	map["KP_7"] = false;
	map["KP_8"] = false;
	map["KP_9"] = false;
	map["KP_DECIMAL"] = false;
	map["KP_DIVIDE"] = false;
	map["KP_MULTIPLY"] = false;
	map["KP_SUBTRACT"] = false;
	map["KP_ADD"] = false;
	map["KP_ENTER"] = false;
	map["KP_EQUAL"] = false;
	map["LEFT_SHIFT"] = false;
	map["LEFT_CONTROL"] = false;
	map["LEFT_ALT"] = false;
	map["LEFT_SUPER"] = false;
	map["RIGHT_SHIFT"] = false;
	map["RIGHT_CONTROL"] = false;
	map["RIGHT_ALT"] = false;
	map["RIGHT_SUPER"] = false;
	map["MENU"] = false;
	return map;
}
std::map<std::string, bool> GameLogic::keyboardMap = generateKeyboardMap();
