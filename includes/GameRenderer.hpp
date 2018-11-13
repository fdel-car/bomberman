#pragma once

#include "Collider.hpp"
#include "Model.hpp"
#include "ShaderProgram.hpp"
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
	void refreshWindow(std::vector<Entity *> &entities);
	void closeWindow(void);
	std::map<std::string, Model *> getModels(void) const;

	bool active;

   private:
	GameRenderer(void);
	static void keyCallback(GLFWwindow *window, int key, int scancode,
							int action, int mods);
	static void errorCallback(int error, const char *description);

	GameRenderer(GameRenderer const &src);

	GameRenderer &operator=(GameRenderer const &rhs);

	void _initShaders(void);
	void _initModels(void);
	// void initProgram(void);
	// void drawGUI(void);
	// void createBorder(void);
	// void createGrid(void);
	// void drawGUI(void);
	// void drawPlayer(Entity *player);
	// void drawSquare(Entity *wall);
	// void makeVAO(GLuint &vbo);

	static GameEngine *_gameEngine;

	// General vars
	GLFWwindow *_window;
	int width = 0;
	int height = 0;
	// int xOffset;
	// int yOffset;
	// int squareSize;
	// float startX;
	// float startY;
	// float squarePercentY;
	// float squarePercentX;

	// Nuklear vars
	GUI *graphicUI;

	// Rendering vars
	std::map<std::string, ShaderProgram *> _shaderPrograms;
	std::map<std::string, Model *> _models;

	glm::mat4 _transform;
	// GLuint vbo;
	// GLuint vao;
	// const char *vertexShader;
	// const char *fragmentShader;
	// GLuint vs;
	// GLuint fs;
	// GLuint shaderProgram;
};
