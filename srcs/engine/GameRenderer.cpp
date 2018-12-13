#include "engine/GameRenderer.hpp"
#include "engine/AGame.hpp"
#include "engine/Entity.hpp"
#include "engine/GameEngine.hpp"
#include "engine/Model.hpp"

extern std::string _srcsDir;
extern std::string _assetsDir;

GameRenderer::GameRenderer(GameEngine *gameEngine, AGame *game)
	: _game(game),
	  _isFullScreen(game->isFullScreen()),
	  _widthRequested(game->getWindowWidth()),
	  _heightRequested(game->getWindowHeight()),
	  _models(std::map<std::string, Model *>()),
	  _toDelete(std::vector<std::string>()) {
	_gameEngine = gameEngine;
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	_initWindow();
}

GameRenderer::~GameRenderer(void) {
	if (_graphicUI) delete _graphicUI;
	for (auto model : _models) delete model.second;
	_models.clear();
	if (_shaderProgram) delete _shaderProgram;
	if (_shadowShaderProgram) delete _shadowShaderProgram;
	if (_skyboxShaderProgram) delete _skyboxShaderProgram;
	if (_window) glfwDestroyWindow(_window);
	glfwTerminate();
	return;
}

void GameRenderer::_initWindow(void) {
	if (_window) glfwDestroyWindow(_window);

	_monitor = glfwGetPrimaryMonitor();
	_mode = glfwGetVideoMode(_monitor);
	if (_isFullScreen) {
		_widthRequested = _mode->width;
		_heightRequested = _mode->height;
	}
	_window = glfwCreateWindow(_widthRequested, _heightRequested, "Bomberman",
							   _isFullScreen ? _monitor : nullptr, nullptr);
	if (!_window) {
		glfwTerminate();
		throw std::runtime_error("Failed to create windows GLFW");
	}
	glfwSetWindowPos(_window, (_mode->width / 2) - (_widthRequested / 2),
					 (_mode->height / 2) - (_heightRequested / 2));
	glfwGetFramebufferSize(_window, &_width, &_height);
	glfwMakeContextCurrent(_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize GLAD");
	glViewport(0, 0, _width, _height);
	glfwSetKeyCallback(_window, keyCallback);
	glfwSetCursorPosCallback(_window, mouseCallback);

	_initGUI();
	_initDepthMap();  // TODO Check if the Framebuffer was create correctly
	_initShader();
}

void GameRenderer::_initGUI() {
	std::vector<std::tuple<float, std::string, std::string>> vFontPath =
		_game->getNeededFont();
	if (_graphicUI) delete _graphicUI;
	_graphicUI = new GUI(this, _window, vFontPath);
}

bool GameRenderer::_initDepthMap(void) {
	// Create framebuffer object for rendering the depth map
	glGenFramebuffers(1, &_depthMapFBO);

	// Create a 2D texture that we'll use as the framebuffer's depth buffer
	glGenTextures(1, &_depthMap);
	glBindTexture(GL_TEXTURE_2D, _depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_W, SHADOW_H, 0,
				 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attach depth texture as the framebuffer's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
						   _depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Failed to initialize Depth Map." << std::endl;
		return false;
	}
	return true;
}

void GameRenderer::_initShader(void) {
	if (_shaderProgram) delete _shaderProgram;
	if (_shadowShaderProgram) delete _shadowShaderProgram;
	if (_skyboxShaderProgram) delete _skyboxShaderProgram;
	_shaderProgram = new ShaderProgram(_srcsDir + "engine/shaders/default.vs",
									   _srcsDir + "engine/shaders/default.fs");
	_shadowShaderProgram =
		new ShaderProgram(_srcsDir + "engine/shaders/depthMap.vs",
						  _srcsDir + "engine/shaders/depthMap.fs");

	_skyboxShaderProgram =
		new ShaderProgram(_srcsDir + "engine/shaders/skybox.vs",
						  _srcsDir + "engine/shaders/skybox.fs");

	glUseProgram(_shaderProgram->getID());
	_shaderProgram->setInt("shadowMap", 0);

	glUseProgram(_shaderProgram->getID());
	_shaderProgram->setInt("diffuseTexture", 1);

	glUseProgram(_skyboxShaderProgram->getID());
	_skyboxShaderProgram->setInt("skybox", 2);
}

void GameRenderer::loadAssets(std::map<std::string, ModelInfo> resources) {
	// Find old models that are no longer needed
	for (auto &elem : _models) {
		if (resources.find(elem.first) == resources.end()) {
			_toDelete.push_back(elem.first);
		}
	}

	// Add new
	for (auto resource : resources) {
		if (_models.find(resource.first) == _models.end()) {
			_models[resource.first] = new Model(resource.second.modelPath);
			for (auto animInfo : resource.second.animMap) {
				_models[resource.first]->addAnimation(animInfo.first,
													  animInfo.second);
			}
		}
	}
}

void GameRenderer::initModelsMeshes(void) {
	// Free models that are no longer used
	for (auto name : _toDelete) {
		if (_models[name] != nullptr) {
			delete _models[name];
		}
		_models.erase(name);
	}
	_toDelete.clear();

	// Init each remaining model
	for (auto model : _models) {
		model.second->initModel();
	}
}

void GameRenderer::getUserInput(void) { glfwPollEvents(); }

void GameRenderer::refreshWindow(std::vector<Entity *> &entities,
								 Camera *camera, Light *light, Skybox *skybox) {
	// Custom OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_lightSpaceMatrix = light->getProjectionMatrix() * light->getViewMatrix();
	// Shadow map
	glUseProgram(_shadowShaderProgram->getID());
	glViewport(0, 0, SHADOW_W, SHADOW_H);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	_shadowShaderProgram->setMat4("lightSpaceMatrix", _lightSpaceMatrix);
	glCullFace(GL_FRONT);
	for (auto entity : entities) {
		if (!entity->doShowModel()) continue;
		_shadowShaderProgram->setMat4("M", entity->getModelMatrix());
		Model *model = entity->getModel();
		if (model && model->isRigged() && entity->shouldBeAnimated)
			model->updateBoneTransforms(
				&entity->currentAnimTime, entity->currentAnimName,
				entity->loopAnim, _gameEngine->getDeltaTime(),
				entity->currentAnimSpeed);
		if (model) model->draw(*_shadowShaderProgram);
	}
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Basic rendering OpenGL state
	glViewport(0, 0, _width, _height);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(_shaderProgram->getID());
	_shaderProgram->setMat4(
		"VP", camera->getProjectionMatrix() * camera->getViewMatrix());
	_shaderProgram->setVec3("viewPos", camera->getPosition());
	_shaderProgram->setVec3("lightDir", light->getDir());
	_shaderProgram->setVec3("lightColor", light->getColor());
	_shaderProgram->setMat4("lightSpaceMatrix", _lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depthMap);
	for (auto entity : entities) {
		if (!entity->doShowModel()) continue;
		_shaderProgram->setMat4("M", entity->getModelMatrix());
		Model *model = entity->getModel();
		if (model && model->isRigged() && entity->shouldBeAnimated)
			model->updateBoneTransforms(
				&entity->currentAnimTime, entity->currentAnimName,
				entity->loopAnim, 0.0, entity->currentAnimSpeed);
		if (model) model->draw(*_shaderProgram, entity->getColor());
	}

	if (skybox != nullptr) {
		// Skybox
		glDepthFunc(GL_LEQUAL);
		glUseProgram(_skyboxShaderProgram->getID());

		_skyboxShaderProgram->setMat4(
			"view", glm::mat4(glm::mat3(camera->getViewMatrix())));
		_skyboxShaderProgram->setMat4("projection",
									  camera->getProjectionMatrix());

		glBindVertexArray(skybox->getVAO());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTexture());
		_skyboxShaderProgram->setInt("skybox", 2);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);  // Set depth function back to default
	}

	// Default OpenGL state
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);

	_graphicUI->nkNewFrame();
	camera->drawGUI(_graphicUI);
	_graphicUI->nkRender();

	// Put everything to screen
	glfwSwapBuffers(_window);
}

void GameRenderer::setNewResolution(bool isFullScreen, int width, int height) {
	if (width <= 0 || height <= 0) return;
	if (isFullScreen == _isFullScreen && width == _widthRequested &&
		height == _heightRequested)
		return;

	if (width > _mode->width) {
		std::cerr
			<< "\033[0;33m:Warning:\033[0m "
			<< "Your screen doesn't support a resolution width bigger than "
			<< _mode->width << " (" << width << " asked)" << std::endl;
		width = _mode->width;
	}
	if (height > _mode->height) {
		std::cerr
			<< "\033[0;33m:Warning:\033[0m "
			<< "Your screen doesn't support a resolution height bigger than "
			<< _mode->height << " (" << height << " asked)" << std::endl;
		height = _mode->height;
	}
	_isFullScreen = isFullScreen;
	_widthRequested = width;
	_heightRequested = height;

	if (_isFullScreen) {
		_widthRequested = _mode->width;
		_heightRequested = _mode->height;
	}

	glfwSetWindowMonitor(_window, _isFullScreen ? _monitor : nullptr,
						 (_mode->width / 2) - (_widthRequested / 2),
						 (_mode->height / 2) - (_heightRequested / 2),
						 _widthRequested, _heightRequested, 60);
	if (!_isFullScreen) {
		// Glfw breaks constraints when it switches back from fullscreen to
		// windowed
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}
	glfwGetFramebufferSize(_window, &_width, &_height);
}

Model *GameRenderer::getModel(std::string modelName) const {
	if (_models.find(modelName) != _models.end()) return _models.at(modelName);
	if (!modelName.empty())
		std::cerr << "\033[0;33m:Warning:\033[0m " << modelName
				  << " not found inside the models map." << std::endl;
	return nullptr;
}

GUI *GameRenderer::getGUI() { return _graphicUI; }

int GameRenderer::getWidth(void) const { return _widthRequested; }

int GameRenderer::getHeight(void) const { return _heightRequested; }

GLFWwindow *GameRenderer::getWindow(void) const { return _window; }

void GameRenderer::errorCallback(int error, const char *description) {
	std::cerr << "Error n." << error << ": " << description << std::endl;
}

void GameRenderer::switchCursorMode(bool debug) const {
	glfwSetInputMode(_window, GLFW_CURSOR,
					 debug ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void GameRenderer::mouseCallback(GLFWwindow *window, double xPos, double yPos) {
	_mousePos.x = xPos;
	_mousePos.y = yPos;
	(void)window;
}

glm::vec2 GameRenderer::_mousePos = glm::vec2();

glm::vec2 GameRenderer::getMousePos(void) const { return _mousePos; }

void GameRenderer::keyCallback(GLFWwindow *window, int key, int scancode,
							   int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_RELEASE) {
		_gameEngine->buttonStateChanged(key, action == GLFW_PRESS);
	}
	(void)scancode;
	(void)window;
	(void)mods;
}

GameEngine *GameRenderer::_gameEngine = nullptr;
