#pragma once

#include "header.hpp"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "GUI/GUI.hpp"

// Custom Defines
#define WHITE_SHADER 1
#define GREEN_SHADER 2
#define RED_SHADER 3
#define CYAN_SHADER 4
#define YELLOW_SHADER 5
#define GRAY_SHADER 6

class GameEngine;

class Entity;

class GameRenderer {
   public:
	GameRenderer(GameEngine *gameEngine);
	~GameRenderer(void);

	void getUserInput(void);
	void refreshWindow(void);
	void closeWindow(void);

	bool active;

   private:
	GameRenderer(void);
	static void keyCallback(GLFWwindow *window, int key, int scancode,
							int action, int mods);
	static void errorCallback(int error, const char *description);

	GameRenderer(GameRenderer const &src);

	GameRenderer &operator=(GameRenderer const &rhs);

	void _initShaders(int type);
	void _initScene(void);
	void initProgram(void);
	// void drawGUI(void);
	void createBorder(void);
	void createGrid(void);
	void makeVAO(GLuint &vbo);
	void drawPlayer(Entity *player);

	static GameEngine *_gameEngine;

	// General vars
	GLFWwindow *_window;
	int width = 0;
	int height = 0;
	int xOffset;
	int yOffset;
	int squareSize;
	float startX;
	float startY;
	float squarePercentY;
	float squarePercentX;

	// Nuklear vars
	GUI *graphicUI;

	// Rendering vars
	GLuint _shaderProgram;
	std::list<unsigned int> _models;
	GLuint vbo;
	GLuint vao;
	const char *vertexShader;
	const char *fragmentShader;
	GLuint vs;
	GLuint fs;
	GLuint shaderProgram;
};
