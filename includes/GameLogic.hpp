#pragma once

#include "AEntity.hpp"
#include "AGameScene.hpp"
#include "AudioManager.hpp"
#include "header.hpp"

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

typedef std::chrono::high_resolution_clock Clock;

class GameRenderer;

class GameLogic {
   private:
	static std::map<std::string, bool> keyboardMap;

	GameLogic(void);
	GameLogic(GameLogic const &src);

	GameLogic &operator=(GameLogic const &rhs);

	bool initScene(int newSceneIdx);
	int renderGame(void);

	// Graphic libraries vars
	GameRenderer *graphicLib;
	int squareSize;
	int xOffset;
	int yOffset;
	Clock::time_point _frameTs;
	Clock::time_point _lastFrameTs;
	double _deltaTime;
	AudioManager *audioManager;

	// Game model vars
	bool running;
	int mapH;
	int mapW;
	bool restartRequest;
	std::tuple<int, int> playerPos;

	// Scene management vars
	int _sceneIdx;
	std::vector<AGameScene *> _gameScenes;
	std::vector<AEntity *> _activeEntities;

   public:
	GameLogic(std::vector<AGameScene *> gameScenes);
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
