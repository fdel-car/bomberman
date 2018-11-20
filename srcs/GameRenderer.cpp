#include "GameRenderer.hpp"
#include "AGame.hpp"
#include "Entity.hpp"
#include "GameEngine.hpp"
#include "Model.hpp"

extern std::string _assetsDir;

GameRenderer::GameRenderer(GameEngine *gameEngine, AGame *game) {
	_gameEngine = gameEngine;
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) throw new std::runtime_error("Failed to initialize GLFW");
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	_window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Bomberman", NULL,
							   NULL);  // Size of screen will change
	if (!_window) {
		glfwTerminate();
		throw new std::runtime_error("Failed to create windows GLFW");
	}
	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(_window, (mode->width / 2) - (WINDOW_W / 2),
					 (mode->height / 2) - (WINDOW_H / 2));
	glfwMakeContextCurrent(_window);
	glfwGetWindowSize(_window, &_width, &_height);
	// glfwSetWindowUserPointer(_window, this);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw new std::runtime_error("Failed to initialize GLAD");
	glViewport(0, 0, WINDOW_W, WINDOW_H);
	glfwSetKeyCallback(_window, keyCallback);

	_initGUI(game);
	_initShader();
	_initModels();
}

GameRenderer::~GameRenderer(void) {
	delete graphicUI;
	for (auto model : _models) delete model.second;
	delete _shaderProgram;
	_models.clear();
	if (_window) glfwDestroyWindow(_window);
	glfwTerminate();
	return;
}

void GameRenderer::_initGUI(AGame *game) {
	std::vector<std::tuple<float, std::string, std::string>> vFontPath =
		game->getNeededFont();
	graphicUI = new GUI(_window, vFontPath);
}

void GameRenderer::_initShader(void) {
	_shaderProgram = new ShaderProgram(_assetsDir + "../srcs/shaders/4.1.vs",
									   _assetsDir + "../srcs/shaders/4.1.fs");
	glUseProgram(_shaderProgram->getID());

	// Init GLint uniform identfiers
	_projectionLoc =
		glGetUniformLocation(_shaderProgram->getID(), "projection");
	_viewLoc = glGetUniformLocation(_shaderProgram->getID(), "view");
	_modelLoc = glGetUniformLocation(_shaderProgram->getID(), "model");
	_lightDirLoc = glGetUniformLocation(_shaderProgram->getID(), "lightDir");
	_viewPosLoc = glGetUniformLocation(_shaderProgram->getID(), "viewPos");
	_lightColorLoc =
		glGetUniformLocation(_shaderProgram->getID(), "lightColor");

	// Set permanent values
	glUniform3fv(_lightDirLoc, 1,
				 glm::value_ptr(glm::normalize(glm::vec3(0.2f, -1.0f, -0.3f))));
	glUniform3fv(_lightColorLoc, 1,
				 glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
}

void GameRenderer::_initModels(void) {
	_models["Cube"] = new Model("cube");
	_models["Player"] = new Model("player");
	_models["Bomb"] = new Model("bomb");
	_models["Enemy"] = new Model("enemy");
}

void GameRenderer::getUserInput(void) { glfwPollEvents(); }

void GameRenderer::refreshWindow(std::vector<Entity *> &entities,
								 Camera *camera) {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(_shaderProgram->getID());
	glUniformMatrix4fv(_viewLoc, 1, GL_FALSE,
					   glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(_projectionLoc, 1, GL_FALSE,
					   glm::value_ptr(camera->getProjectionMatrix()));
	glUniform3fv(_viewPosLoc, 1, glm::value_ptr(camera->getPosition()));

	for (auto entity : entities) {
		glUniformMatrix4fv(_modelLoc, 1, GL_FALSE,
						   glm::value_ptr(entity->getModelMatrix()));
		for (auto mesh : entity->getModel()->getMeshes()) {
			glBindVertexArray(mesh->VAO);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, mesh->getSize());
		}
	}
	// Default OpenGL state
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);

	graphicUI->nkNewFrame();
	camera->drawGUI(graphicUI);
	graphicUI->nkRender();

	// Put everything to screen
	glfwSwapBuffers(_window);
}

Model *GameRenderer::getModel(std::string modelName) const {
	if (_models.find(modelName) != _models.end()) return _models.at(modelName);
	return nullptr;
}

GUI *GameRenderer::getGUI() { return graphicUI; }

int GameRenderer::getWidth(void) const { return _width; }

int GameRenderer::getHeight(void) const { return _height; }

void GameRenderer::errorCallback(int error, const char *description) {
	std::cerr << "Error n." << error << ": " << description << std::endl;
}

void GameRenderer::keyCallback(GLFWwindow *window, int key, int scancode,
							   int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_UNKNOWN:
				_gameEngine->buttonStateChanged("UNKNOWN",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_SPACE:
				_gameEngine->buttonStateChanged("SPACE", action == GLFW_PRESS);
				break;
			case GLFW_KEY_APOSTROPHE:
				_gameEngine->buttonStateChanged("APOSTROPHE",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_COMMA:
				_gameEngine->buttonStateChanged("COMMA", action == GLFW_PRESS);
				break;
			case GLFW_KEY_MINUS:
				_gameEngine->buttonStateChanged("MINUS", action == GLFW_PRESS);
				break;
			case GLFW_KEY_PERIOD:
				_gameEngine->buttonStateChanged("PERIOD", action == GLFW_PRESS);
				break;
			case GLFW_KEY_SLASH:
				_gameEngine->buttonStateChanged("SLASH", action == GLFW_PRESS);
				break;
			case GLFW_KEY_0:
				_gameEngine->buttonStateChanged("0", action == GLFW_PRESS);
				break;
			case GLFW_KEY_1:
				_gameEngine->buttonStateChanged("1", action == GLFW_PRESS);
				break;
			case GLFW_KEY_2:
				_gameEngine->buttonStateChanged("2", action == GLFW_PRESS);
				break;
			case GLFW_KEY_3:
				_gameEngine->buttonStateChanged("3", action == GLFW_PRESS);
				break;
			case GLFW_KEY_4:
				_gameEngine->buttonStateChanged("4", action == GLFW_PRESS);
				break;
			case GLFW_KEY_5:
				_gameEngine->buttonStateChanged("5", action == GLFW_PRESS);
				break;
			case GLFW_KEY_6:
				_gameEngine->buttonStateChanged("6", action == GLFW_PRESS);
				break;
			case GLFW_KEY_7:
				_gameEngine->buttonStateChanged("7", action == GLFW_PRESS);
				break;
			case GLFW_KEY_8:
				_gameEngine->buttonStateChanged("8", action == GLFW_PRESS);
				break;
			case GLFW_KEY_9:
				_gameEngine->buttonStateChanged("9", action == GLFW_PRESS);
				break;
			case GLFW_KEY_SEMICOLON:
				_gameEngine->buttonStateChanged(";", action == GLFW_PRESS);
				break;
			case GLFW_KEY_EQUAL:
				_gameEngine->buttonStateChanged("EQUAL", action == GLFW_PRESS);
				break;
			case GLFW_KEY_A:
				_gameEngine->buttonStateChanged("A", action == GLFW_PRESS);
				break;
			case GLFW_KEY_B:
				_gameEngine->buttonStateChanged("B", action == GLFW_PRESS);
				break;
			case GLFW_KEY_C:
				_gameEngine->buttonStateChanged("C", action == GLFW_PRESS);
				break;
			case GLFW_KEY_D:
				_gameEngine->buttonStateChanged("D", action == GLFW_PRESS);
				break;
			case GLFW_KEY_E:
				_gameEngine->buttonStateChanged("E", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F:
				_gameEngine->buttonStateChanged("F", action == GLFW_PRESS);
				break;
			case GLFW_KEY_G:
				_gameEngine->buttonStateChanged("G", action == GLFW_PRESS);
				break;
			case GLFW_KEY_H:
				_gameEngine->buttonStateChanged("H", action == GLFW_PRESS);
				break;
			case GLFW_KEY_I:
				_gameEngine->buttonStateChanged("I", action == GLFW_PRESS);
				break;
			case GLFW_KEY_J:
				_gameEngine->buttonStateChanged("L", action == GLFW_PRESS);
				break;
			case GLFW_KEY_K:
				_gameEngine->buttonStateChanged("K", action == GLFW_PRESS);
				break;
			case GLFW_KEY_L:
				_gameEngine->buttonStateChanged("L", action == GLFW_PRESS);
				break;
			case GLFW_KEY_M:
				_gameEngine->buttonStateChanged("M", action == GLFW_PRESS);
				break;
			case GLFW_KEY_N:
				_gameEngine->buttonStateChanged("N", action == GLFW_PRESS);
				break;
			case GLFW_KEY_O:
				_gameEngine->buttonStateChanged("O", action == GLFW_PRESS);
				break;
			case GLFW_KEY_P:
				_gameEngine->buttonStateChanged("P", action == GLFW_PRESS);
				break;
			case GLFW_KEY_Q:
				_gameEngine->buttonStateChanged("Q", action == GLFW_PRESS);
				break;
			case GLFW_KEY_R:
				_gameEngine->buttonStateChanged("R", action == GLFW_PRESS);
				break;
			case GLFW_KEY_S:
				_gameEngine->buttonStateChanged("S", action == GLFW_PRESS);
				break;
			case GLFW_KEY_T:
				_gameEngine->buttonStateChanged("T", action == GLFW_PRESS);
				break;
			case GLFW_KEY_U:
				_gameEngine->buttonStateChanged("U", action == GLFW_PRESS);
				break;
			case GLFW_KEY_V:
				_gameEngine->buttonStateChanged("V", action == GLFW_PRESS);
				break;
			case GLFW_KEY_W:
				_gameEngine->buttonStateChanged("W", action == GLFW_PRESS);
				break;
			case GLFW_KEY_X:
				_gameEngine->buttonStateChanged("X", action == GLFW_PRESS);
				break;
			case GLFW_KEY_Y:
				_gameEngine->buttonStateChanged("Y", action == GLFW_PRESS);
				break;
			case GLFW_KEY_Z:
				_gameEngine->buttonStateChanged("Z", action == GLFW_PRESS);
				break;
			case GLFW_KEY_LEFT_BRACKET:
				_gameEngine->buttonStateChanged("[", action == GLFW_PRESS);
				break;
			case GLFW_KEY_BACKSLASH:
				_gameEngine->buttonStateChanged("\\", action == GLFW_PRESS);
				break;
			case GLFW_KEY_RIGHT_BRACKET:
				_gameEngine->buttonStateChanged("]", action == GLFW_PRESS);
				break;
			case GLFW_KEY_GRAVE_ACCENT:
				_gameEngine->buttonStateChanged("`", action == GLFW_PRESS);
				break;
			case GLFW_KEY_WORLD_1:
				_gameEngine->buttonStateChanged("WORLD_1",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_WORLD_2:
				_gameEngine->buttonStateChanged("WORLD_2",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_ESCAPE:
				_gameEngine->buttonStateChanged("ESCAPE", action == GLFW_PRESS);
				break;
			case GLFW_KEY_ENTER:
				_gameEngine->buttonStateChanged("ENTER", action == GLFW_PRESS);
				break;
			case GLFW_KEY_TAB:
				_gameEngine->buttonStateChanged("TAB", action == GLFW_PRESS);
				break;
			case GLFW_KEY_BACKSPACE:
				_gameEngine->buttonStateChanged("BACKSPACE",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_INSERT:
				_gameEngine->buttonStateChanged("INSERT", action == GLFW_PRESS);
				break;
			case GLFW_KEY_DELETE:
				_gameEngine->buttonStateChanged("DELETE", action == GLFW_PRESS);
				break;
			case GLFW_KEY_RIGHT:
				_gameEngine->buttonStateChanged("RIGHT", action == GLFW_PRESS);
				break;
			case GLFW_KEY_LEFT:
				_gameEngine->buttonStateChanged("LEFT", action == GLFW_PRESS);
				break;
			case GLFW_KEY_DOWN:
				_gameEngine->buttonStateChanged("DOWN", action == GLFW_PRESS);
				break;
			case GLFW_KEY_UP:
				_gameEngine->buttonStateChanged("UP", action == GLFW_PRESS);
				break;
			case GLFW_KEY_PAGE_UP:
				_gameEngine->buttonStateChanged("PAGE_UP",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_PAGE_DOWN:
				_gameEngine->buttonStateChanged("PAGE_DOWN",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_HOME:
				_gameEngine->buttonStateChanged("HOME", action == GLFW_PRESS);
				break;
			case GLFW_KEY_END:
				_gameEngine->buttonStateChanged("END", action == GLFW_PRESS);
				break;
			case GLFW_KEY_CAPS_LOCK:
				_gameEngine->buttonStateChanged("CAPS_LOCK",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_SCROLL_LOCK:
				_gameEngine->buttonStateChanged("SCROLL_LOCK",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_NUM_LOCK:
				_gameEngine->buttonStateChanged("NUM_LOCK",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_PRINT_SCREEN:
				_gameEngine->buttonStateChanged("PRINT_SCREEN",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_PAUSE:
				_gameEngine->buttonStateChanged("PAUSE", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F1:
				_gameEngine->buttonStateChanged("F1", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F2:
				_gameEngine->buttonStateChanged("F2", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F3:
				_gameEngine->buttonStateChanged("F3", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F4:
				_gameEngine->buttonStateChanged("F4", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F5:
				_gameEngine->buttonStateChanged("F5", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F6:
				_gameEngine->buttonStateChanged("F6", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F7:
				_gameEngine->buttonStateChanged("F7", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F8:
				_gameEngine->buttonStateChanged("F8", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F9:
				_gameEngine->buttonStateChanged("F9", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F10:
				_gameEngine->buttonStateChanged("F10", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F11:
				_gameEngine->buttonStateChanged("F11", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F12:
				_gameEngine->buttonStateChanged("F12", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F13:
				_gameEngine->buttonStateChanged("F13", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F14:
				_gameEngine->buttonStateChanged("F14", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F15:
				_gameEngine->buttonStateChanged("F15", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F16:
				_gameEngine->buttonStateChanged("F16", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F17:
				_gameEngine->buttonStateChanged("F17", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F18:
				_gameEngine->buttonStateChanged("F18", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F19:
				_gameEngine->buttonStateChanged("F19", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F20:
				_gameEngine->buttonStateChanged("F20", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F21:
				_gameEngine->buttonStateChanged("F21", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F22:
				_gameEngine->buttonStateChanged("F22", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F23:
				_gameEngine->buttonStateChanged("F23", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F24:
				_gameEngine->buttonStateChanged("F24", action == GLFW_PRESS);
				break;
			case GLFW_KEY_F25:
				_gameEngine->buttonStateChanged("F25", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_0:
				_gameEngine->buttonStateChanged("KP_0", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_1:
				_gameEngine->buttonStateChanged("KP_1", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_2:
				_gameEngine->buttonStateChanged("KP_2", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_3:
				_gameEngine->buttonStateChanged("KP_3", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_4:
				_gameEngine->buttonStateChanged("KP_4", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_5:
				_gameEngine->buttonStateChanged("KP_5", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_6:
				_gameEngine->buttonStateChanged("KP_6", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_7:
				_gameEngine->buttonStateChanged("KP_7", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_8:
				_gameEngine->buttonStateChanged("KP_8", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_9:
				_gameEngine->buttonStateChanged("KP_9", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_DECIMAL:
				_gameEngine->buttonStateChanged("KP_DECIMAL",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_DIVIDE:
				_gameEngine->buttonStateChanged("KP_DIVIDE",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_MULTIPLY:
				_gameEngine->buttonStateChanged("KP_MULTIPLY",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_SUBTRACT:
				_gameEngine->buttonStateChanged("KP_SUBTRACT",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_ADD:
				_gameEngine->buttonStateChanged("KP_ADD", action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_ENTER:
				_gameEngine->buttonStateChanged("KP_ENTER",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_KP_EQUAL:
				_gameEngine->buttonStateChanged("KP_EQUAL",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_LEFT_SHIFT:
				_gameEngine->buttonStateChanged("LEFT_SHIFT",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_LEFT_CONTROL:
				_gameEngine->buttonStateChanged("LEFT_CONTROL",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_LEFT_ALT:
				_gameEngine->buttonStateChanged("LEFT_ALT",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_LEFT_SUPER:
				_gameEngine->buttonStateChanged("LEFT_SUPER",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_RIGHT_SHIFT:
				_gameEngine->buttonStateChanged("RIGHT_SHIFT",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_RIGHT_CONTROL:
				_gameEngine->buttonStateChanged("RIGHT_CONTROL",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_RIGHT_ALT:
				_gameEngine->buttonStateChanged("RIGHT_ALT",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_RIGHT_SUPER:
				_gameEngine->buttonStateChanged("RIGHT_SUPER",
												action == GLFW_PRESS);
				break;
			case GLFW_KEY_MENU:
				_gameEngine->buttonStateChanged("MENU", action == GLFW_PRESS);
				break;
		}
	}
	(void)scancode;
	(void)window;
	(void)mods;
}

GameEngine *GameRenderer::_gameEngine = NULL;
