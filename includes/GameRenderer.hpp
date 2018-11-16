#pragma once

#include "Camera.hpp"
#include "Collider.hpp"
#include "Model.hpp"
#include "ShaderProgram.hpp"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "GUI/GUI.hpp"

// // Custom Defines
// #define WHITE_SHADER 1
// #define GREEN_SHADER 2
// #define RED_SHADER 3
// #define CYAN_SHADER 4
// #define YELLOW_SHADER 5
// #define GRAY_SHADER 6

class GameEngine;

class Entity;

class GameRenderer {
   public:
	GameRenderer(GameEngine *gameEngine);
	~GameRenderer(void);

	void getUserInput(void);
	void refreshWindow(std::vector<Entity *> &entities, Camera *camera);
	void closeWindow(void);
	Model *getModel(std::string modelName) const;
	int getWidth(void) const;
	int getHeight(void) const;

	bool active;

   private:
	GameRenderer(void);
	static void keyCallback(GLFWwindow *window, int key, int scancode,
							int action, int mods);
	static void errorCallback(int error, const char *description);

	GameRenderer(GameRenderer const &src);

	GameRenderer &operator=(GameRenderer const &rhs);

	void _initShader(void);
	void _initModels(void);
	// void drawGUI(void);
	static GameEngine *_gameEngine;

	// General vars
	GLFWwindow *_window;
	int _width = 0;
	int _height = 0;

	// Nuklear vars
	GUI *graphicUI;

	// Rendering vars
	ShaderProgram *_shaderProgram;
	std::map<std::string, Model *> _models;

	// Camera
	glm::mat4 _projection;
	GLint _projectionLoc;
	GLint _viewLoc;
	GLint _modelLoc;
	GLint _lightDirLoc;
	GLint _viewPosLoc;
	GLint _lightColorLoc;
};
