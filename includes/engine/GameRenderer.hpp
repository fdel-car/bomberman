#pragma once

#include "engine/Camera.hpp"
#include "engine/Collider.hpp"
#include "engine/Light.hpp"
#include "engine/Model.hpp"
#include "engine/ShaderProgram.hpp"
#include "engine/Skybox.hpp"

#include "GUI/GUI.hpp"

// // Custom Defines
// #define WHITE_SHADER 1
// #define GREEN_SHADER 2
// #define RED_SHADER 3
// #define CYAN_SHADER 4
// #define YELLOW_SHADER 5
// #define GRAY_SHADER 6

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
	void setNewResolution(bool isFullscreen, int width, int height);

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

	void _initGUI(AGame *game);
	bool _initDepthMap(void);
	void _initShader(void);
	void _initModels(void);

	// void _initCubeMap(void);
	// void _initSkyboxFaces(void);

	static GameEngine *_gameEngine;
	static glm::vec2 _mousePos;

	// General vars
	GLFWwindow *_window;
	bool _isFullScreen;
	int _width;
	int _height;

	// Nuklear vars
	GUI *graphicUI;

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

	// // Skybox
	// std::vector<std::string> _faces;
	// GLuint	_skyboxTexture;
	// GLuint _skyboxVAO;
	// GLuint _skyboxVBO;
};
