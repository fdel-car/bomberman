#pragma once

#include "header.hpp"
#include "AudioManager.hpp"
#include "AEntity.hpp"

#define MAP_SIZE 40

#define WINDOW_W 1100
#define WINDOW_H 1200
#define WINDOW_MIN_X_OFFSET 50
#define WINDOW_MIN_Y_OFFSET 100
#define MIN_SQUARE_SIZE 10
#define MAX_SQUARE_SIZE 25
#define OUTLINE_TICKNESS 2

#define KEY_0 "0"
#define KEY_W "W"
#define KEY_A "A"
#define KEY_S "S"
#define KEY_D "D"
#define KEY_W_LOWER "w"
#define KEY_A_LOWER "a"
#define KEY_S_LOWER "s"
#define KEY_D_LOWER "d"
#define KEY_LEFT "Left"
#define KEY_UP "Up"
#define KEY_RIGHT "Right"
#define KEY_DOWN "Down"

#define KEY_ESCAPE "Escape"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define FRAME_TIME 0.1f

class GameRenderer;

class GameLogic
{
  private:
	static const std::list<std::string> changeLibraryKeys;
	static const std::vector<std::tuple<std::string, int>> changeDirectionKeys;

	GameLogic(GameLogic const &src);

	GameLogic &operator=(GameLogic const &rhs);

	void printUsage(void);
	void changeLibraryRequest(std::string key_code);
	void updateGameState(void);
	int renderGame(void);
	void changeDirectionTo(int &player_direction, int &player_direction_requested, int newDir);

	void initPlayer(void);
	bool playerCanMove(void);
	void movePlayer(std::tuple<int, int> &player_body, int &player_dir);

	// Graphic libraries vars
	GameRenderer *graphicLib;
	int squareSize;
	int xOffset;
	int yOffset;
	int dlIndex;
	int dlPastIndex;
	time_t timer;
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
	void buttonPressed(const char *button);
	void buttonStateChanged(const char *button, bool isPressed);

	int getSquareSize(void);
	int getXOffset(void);
	int getYOffset(void);
	int getMapW(void);
	int getMapH(void);
	int getPlayerDirection(void);
	bool getIsPlayerAlive(void);
	std::tuple<int, int> &getPlayerPos(void);

	bool canRun;
};
