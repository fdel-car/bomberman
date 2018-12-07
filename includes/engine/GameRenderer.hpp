#pragma once

#include "engine/Camera.hpp"
#include "engine/Collider.hpp"
#include "engine/Light.hpp"
#include "engine/Model.hpp"
#include "engine/ShaderProgram.hpp"
#include "engine/Skybox.hpp"

#include "GUI/GUI.hpp"

#define SHADOW_H 4096
#define SHADOW_W 4096

class GameEngine;

class Entity;

class AGame;

class GameRenderer final {
   public:
	GameRenderer(GameEngine *gameEngine, AGame *game);
	~GameRenderer(void);

	void getUserInput(void);
	void switchCursorMode(bool debug) const;
	void refreshWindow(std::vector<Entity *> &entities, Camera *camera,
					   Light *light, Skybox *skybox);
	void setNewResolution(bool isFullScreen, int width, int height);

	Model *getModel(std::string modelName) const;
	int getWidth(void) const;
	int getHeight(void) const;
	glm::vec2 getMousePos(void) const;
	GUI *getGUI();
	GLFWwindow *getWindow(void) const;

	bool active;

   private:
	static void keyCallback(GLFWwindow *window, int key, int scancode,
							int action, int mods);
	static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
	static void errorCallback(int error, const char *description);

	GameRenderer(void);
	GameRenderer(GameRenderer const &src);

	GameRenderer &operator=(GameRenderer const &rhs);

	void _initWindow(void);
	void _initGUI(void);
	bool _initDepthMap(void);
	void _initShader(void);
	void _initModels(void);

	static GameEngine *_gameEngine;
	static glm::vec2 _mousePos;

	// General vars
	GLFWwindow *_window;
	AGame *_game;
	bool _isFullScreen;
	int _width;
	int _height;
	int _widthRequested;
	int _heightRequested;
	GLFWmonitor *_monitor;
	const GLFWvidmode *_mode;

	// Nuklear vars
	GUI *_graphicUI;

	// Rendering vars
	ShaderProgram *_shaderProgram;
	ShaderProgram *_shadowShaderProgram;
	ShaderProgram *_skyboxShaderProgram;
	std::map<std::string, Model *> _models;

	// Shadow
	GLuint _depthMapFBO;
	GLuint _depthMap;

	// Light
	glm::mat4 _lightSpaceMatrix;
};
