#pragma once

#include "header.hpp"
#include "AudioManager.hpp"
#include "AEntity.hpp"

#define MAP_SIZE 40

#define WINDOW_W 1280
#define WINDOW_H 720
#define WINDOW_MIN_X_OFFSET 50
#define WINDOW_MIN_Y_OFFSET 100
#define MIN_SQUARE_SIZE 10
#define MAX_SQUARE_SIZE 25
#define OUTLINE_TICKNESS 2

#define KEY_W "W"
#define KEY_A "A"
#define KEY_S "S"
#define KEY_D "D"
#define KEY_LEFT "LEFT"
#define KEY_UP "UP"
#define KEY_RIGHT "RIGHT"
#define KEY_DOWN "DOWN"
#define KEY_ESCAPE "ESCAPE"

class GameRenderer;

class GameLogic
{
private:
	static std::map<std::string, bool> keyboardMap;

	GameLogic(GameLogic const &src);

	GameLogic &operator=(GameLogic const &rhs);

	void printUsage(void);
	int renderGame(void);
	void changeDirectionTo(int &playerDirection, int &playerDirectionRequested, int newDir);

	// Graphic libraries vars
	GameRenderer *graphicLib;
	int squareSize;
	int xOffset;
	int yOffset;
	clock_t timer;
	double pastFrameLength;
	AudioManager *audioManager;

	// Game model vars
	bool running;
	int mapH;
	int mapW;
	bool isPlayerAlive;
	bool hasShownDeath;
	int playerDirection;
	int playerDirectionRequested;
	bool restartRequest;
	std::tuple<int, int> playerPos;
	std::chrono::high_resolution_clock::time_point creationPreciseTime;

	std::vector<AEntity *> entities;

public:
	GameLogic(void);
	~GameLogic(void);

	int run();

	// Functions needed by Renderer
	int getSquareSize(void);
	int getXOffset(void);
	int getYOffset(void);
	int getMapW(void);
	int getMapH(void);
	AEntity *getFirstEntityWithName(std::string entityName);
	// std::vector<AEntity *> getEntitiesWithName(std::string entityName);
	// AEntity *getFirstEntityWithLabel(std::string entityLabel);
	// std::vector<AEntity *> getEntitiesWithLabel(std::string entityLabel);
	void buttonStateChanged(std::string button, bool isPressed);

	// Functions needed by entities
	bool isKeyPressed(std::string keyName);
	double getDeltaTime();

	bool canRun;
};
