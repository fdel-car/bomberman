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

struct ModelInfo;

class GameRenderer final {
   public:
	GameRenderer(GameEngine *gameEngine, AGame *game);
	~GameRenderer(void);

	void getUserInput(void);
	void switchCursorMode(bool debug) const;
	void refreshWindow(std::vector<Entity *> &entities, Camera *camera,
					   Light *light, Skybox *skybox);
	void setNewResolution(bool isFullScreen, int width, int height);
	void loadAssets(std::map<std::string, ModelInfo> assets);
	void initModelsMeshes(void);

	Model *getModel(std::string modelName) const;
	int getWidth(void) const;
	int getHeight(void) const;
	glm::vec2 getMousePos(void) const;
	GUI *getGUI();
	GLFWwindow *getWindow(void) const;

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
	GLFWwindow *_window = nullptr;
	AGame *_game = nullptr;
	bool _isFullScreen;
	int _width;
	int _height;
	int _widthRequested;
	int _heightRequested;
	GLFWmonitor *_monitor = nullptr;
	const GLFWvidmode *_mode = nullptr;

	// Nuklear vars
	GUI *_graphicUI = nullptr;

	// Rendering vars
	ShaderProgram *_shaderProgram = nullptr;
	ShaderProgram *_shadowShaderProgram = nullptr;
	ShaderProgram *_skyboxShaderProgram = nullptr;
	std::map<std::string, Model *> _models = std::map<std::string, Model *>();
	std::vector<std::string> _toDelete;  // Models to delete

	// Shadow
	GLuint _depthMapFBO;
	GLuint _depthMap;

	// Light
	glm::mat4 _lightSpaceMatrix;
};
