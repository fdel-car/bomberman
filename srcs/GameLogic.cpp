#include "GameLogic.hpp"
#include "GameRenderer.hpp"

#include "Player.hpp" // TODO: this should be done by a "game initializer" and not by the game engine!

GameLogic::GameLogic()
{
	canRun = false;
	hasShownDeath = false;
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
	std::vector<float> pos(3, 0);
	pos[0] = mapW / 2;
	pos[2] = mapW / 2;
	std::vector<float> rot(4, 0);
	entities.push_back(new Player(this, pos, rot));

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

AEntity *GameLogic::getFirstEntityWithName(std::string entityName)
{
	AEntity *foundElem = nullptr;
	for (auto entity : entities)
	{
		if (entity->name.compare(entityName) == 0)
		{
			foundElem = entity;
			break;
		}
	}
	return foundElem;
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

int GameLogic::renderGame(void)
{
	// Draw window with game infos
	if (graphicLib && !hasShownDeath)
	{
		graphicLib->refreshWindow();
		if (!isPlayerAlive)
			hasShownDeath = true;
	}

	return EXIT_SUCCESS;
}

int GameLogic::run(void)
{
	if (!canRun)
		return EXIT_FAILURE;
	int guiRet;

	// init vars
	running = true;
	restartRequest = false;
	timer = clock();

	audioManager->playStartSound();

	// Start game loop
	while (running)
	{
		// Get delta time in order to synch entities positions
		pastFrameLength = static_cast<float>(clock()) / CLOCKS_PER_SEC - static_cast<float>(timer) / CLOCKS_PER_SEC;
		timer = clock();

		// Update inputs
		graphicLib->getUserInput();

		// TODO: Update game engine statuses (ex. when to quit)
		if (running)
		{
			running = !keyboardMap["ESCAPE"];
		}

		// Update game entities states
		for (auto entity : entities)
		{
			entity->Update();
		}

		// TODO: set position of entities back to prev frame when they collide

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
	if (keyboardMap.find(buttonName) == keyboardMap.end())
	{
		std::runtime_error("Unkown Mapping for '" + buttonName + "' !");
	}
	keyboardMap[buttonName] = isPressed;
}

bool GameLogic::isKeyPressed(std::string keyName)
{
	return keyboardMap[keyName];
}

double GameLogic::getDeltaTime(void)
{
	return pastFrameLength;
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
