#include "GameRenderer.hpp"
#include "Entity.hpp"
#include "GameEngine.hpp"
#include "Shape.hpp"

GameRenderer::GameRenderer(GameEngine *gameEngine) {
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
	glfwSetWindowUserPointer(_window, this);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw new std::runtime_error("Failed to initialize GLAD");
	glViewport(0, 0, WINDOW_W, WINDOW_H);
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(_window, keyCallback);
	// squareSize = _gameEngine->getSquareSize();
	// xOffset = _gameEngine->getXOffset();
	// yOffset = _gameEngine->getYOffset();

	// // Get top left of game screen
	// startX = -((WINDOW_W / 2.0f) - xOffset) / (WINDOW_W / 2.0f);
	// startY = ((WINDOW_H / 2.0f) - yOffset) / (WINDOW_H / 2.0f);

	// // Get the size of each square in the game screen
	// squarePercentY = startY / (_gameEngine->getMapH() / 2.0f);
	// squarePercentX = (-startX) / (_gameEngine->getMapW() / 2.0f);

	// Nuklear init
	// graphicUI = new GUI(window);

	_initShaders();
	_initShapes();
}

GameRenderer::~GameRenderer(void) {
	delete graphicUI;
	closeWindow();
}

// void GameRenderer::makeVAO(GLuint &vbo) {
// 	vao = 0;
// 	glGenVertexArrays(1, &vao);
// 	glBindVertexArray(vao);
// 	glEnableVertexAttribArray(0);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
// }

void GameRenderer::_initShaders(void) {
	_shaderPrograms["4.1"] =
		new ShaderProgram("assets/shaders/4.1.vs", "assets/shaders/4.1.fs");
	glUseProgram(_shaderPrograms["4.1"]->getID());

	// Init projection matrix
	float const aspectRatio = (float)_width / (float)_height;
	// _projection = glm::perspective(glm::radians(45.0f), aspectRatio,
	// 0.1f, 100.0f);
	_projection = glm::ortho(-aspectRatio * 10.0f, aspectRatio * 10.0f, -10.0f,
							 10.0f, 0.1f, 100.0f);

	// Init GLint uniform identfiers
	_projectionLoc =
		glGetUniformLocation(_shaderPrograms["4.1"]->getID(), "projection");
	glUniformMatrix4fv(_projectionLoc, 1, GL_FALSE,
					   glm::value_ptr(_projection));
	_viewLoc = glGetUniformLocation(_shaderPrograms["4.1"]->getID(), "view");
	_modelLoc = glGetUniformLocation(_shaderPrograms["4.1"]->getID(), "model");
	
	_lightDirLoc = glGetUniformLocation(_shaderPrograms["4.1"]->getID(), "lightDire");
	_viewPosLoc = glGetUniformLocation(_shaderPrograms["4.1"]->getID(), "viewPos");
	_lightColorLoc = glGetUniformLocation(_shaderPrograms["4.1"]->getID(), "lightColor");

	// // Shader pour les vertex
	// vertexShader =
	// 	"#version 400\n"
	// 	"in vec3 vp;"
	// 	"void main() {"
	// 	"  gl_Position = vec4(vp, 1.0);"
	// 	"}";

	// if (type == WHITE_SHADER) {
	// 	// Shader pour dessiner ce qu'il y a entre les vertex
	// 	fragmentShader =
	// 		"#version 400\n"
	// 		"out vec4 frag_colour;"
	// 		"void main() {"
	// 		"  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);"
	// 		"}";
	// } else if (type == CYAN_SHADER) {
	// 	// Shader pour dessiner ce qu'il y a entre les vertex
	// 	fragmentShader =
	// 		"#version 400\n"
	// 		"out vec4 frag_colour;"
	// 		"void main() {"
	// 		"  frag_colour = vec4(0.0, 0.9, 0.7, 1.0);"
	// 		"}";
	// } else if (type == RED_SHADER) {
	// 	// Shader pour dessiner ce qu'il y a entre les vertex
	// 	fragmentShader =
	// 		"#version 400\n"
	// 		"out vec4 frag_colour;"
	// 		"void main() {"
	// 		"  frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
	// 		"}";
	// } else if (type == GREEN_SHADER) {
	// 	// Shader pour dessiner ce qu'il y a entre les vertex
	// 	fragmentShader =
	// 		"#version 400\n"
	// 		"out vec4 frag_colour;"
	// 		"void main() {"
	// 		"  frag_colour = vec4(0.0, 0.9, 0.0, 1.0);"
	// 		"}";
	// } else if (type == YELLOW_SHADER) {
	// 	// Shader pour dessiner ce qu'il y a entre les vertex
	// 	fragmentShader =
	// 		"#version 400\n"
	// 		"out vec4 frag_colour;"
	// 		"void main() {"
	// 		"  frag_colour = vec4(1.0, 1.0, 0.0, 1.0);"
	// 		"}";
	// } else if (type == GRAY_SHADER) {
	// 	// Shader pour dessiner ce qu'il y a entre les vertex
	// 	fragmentShader =
	// 		"#version 400\n"
	// 		"out vec4 frag_colour;"
	// 		"void main() {"
	// 		"  frag_colour = vec4(185.0/255.0, 185.0/255.0, 146.0/255.0, 1.0);"
	// 		"}";
	// }

	// vs = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(vs, 1, &vertexShader, NULL);
	// glCompileShader(vs);
	// fs = glCreateShader(GL_FRAGMENT_SHADER);
	// glShaderSource(fs, 1, &fragmentShader, NULL);
	// glCompileShader(fs);

	// // check if shaders compile fail
	// GLint status;
	// glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	// if (status != GL_TRUE)
	// 	std::cout << "Vertex Shader compile failed." << std::endl;
	// glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	// if (status != GL_TRUE)
	// 	std::cout << "Fragment Shader compile failed." << std::endl;
}

void GameRenderer::_initShapes(void) {
	_shapes["Cube"] = new Shape("assets/objs/cube/cube.obj");
	_shapes["Player"] = new Shape("assets/objs/player/player.obj");
	// _shapes["Bomb"] = new Shape("assets/objs/bomb/bomb.obj");
}

void GameRenderer::getUserInput(void) { glfwPollEvents(); }

void GameRenderer::refreshWindow(std::vector<Entity *> &entities,
								 Camera *camera) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// createBorder();
	// for (const auto entity : entities) {
	// 	// Just a temporary if, waiting to have a way to handle .obj
	// 	if (entity->getCollider()->shape == Collider::Circle)
	// 		drawPlayer(entity);
	// 	else
	// 		drawSquare(entity);
	// }
	// createGrid();
	// graphicUI->drawGUI();

	// glfwSetWindowTitle(_window,
	// 				   toString(static_cast<int>(1 /
	// _gameEngine->getDeltaTime())).c_str());

	glUseProgram(_shaderPrograms["4.1"]->getID());

	// glm::mat4 view = glm::mat4(1.0f);
	// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
	glUniformMatrix4fv(_viewLoc, 1, GL_FALSE,
					   glm::value_ptr(camera->getViewMatrix()));
	glUniform3fv(_viewPosLoc, 1, glm::value_ptr(camera->getPosition()));
	glUniform3fv(_lightDirLoc, 1, glm::value_ptr(glm::vec3( -0.2f, -1.0f, -0.3f )));
	glUniform3fv(_lightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	
	for (auto entity : entities) {
		// entity->rotate(glm::vec3(0.0, 1.0, 0.0), 1.0);
		glUniformMatrix4fv(_modelLoc, 1, GL_FALSE,
						   glm::value_ptr(entity->getModelMatrix()));
		glBindVertexArray((entity->getShape())->getVAO());
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, entity->getShape()->getSize());
		// glBindVertexArray(0);
	}

	(void)entities;
	// Put everything to screen
	glfwSwapBuffers(_window);
}

Shape *GameRenderer::getShape(std::string shapeName) const {
	if (_shapes.find(shapeName) != _shapes.end()) return _shapes.at(shapeName);
	return nullptr;
}

void GameRenderer::closeWindow() {
	if (_window) glfwDestroyWindow(_window);
	glfwTerminate();
}

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
			default:
				break;
		}
	}
	(void)scancode;
	(void)window;
	(void)mods;
}

GameEngine *GameRenderer::_gameEngine = NULL;

// void GameRenderer::createBorder(void) {
// 	float epsilonX = 1 / (WINDOW_W / 2.0f);
// 	float epsilonY = 1 / (WINDOW_H / 2.0f);

// 	float vertexBorders[] = {
// 		startX,
// 		startY + epsilonY,
// 		0.0f,  // top-left
// 		-(startX) + epsilonX,
// 		startY + epsilonY,
// 		0.0f,  // top-right

// 		-(startX) + epsilonX,
// 		startY + epsilonY,
// 		0.0f,  // top-right
// 		-(startX) + epsilonX,
// 		-(startY),
// 		0.0f,  // bottom-right

// 		-(startX) + epsilonX,
// 		-(startY),
// 		0.0f,  // bottom-right
// 		startX,
// 		-(startY),
// 		0.0f,  // bottom-left

// 		startX,
// 		-(startY),
// 		0.0f,  // bottom-left
// 		startX,
// 		startY + epsilonY,
// 		0.0f  // top-left
// 	};
// 	// BUFFER
// 	vbo = 0;
// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBorders), vertexBorders,
// 				 GL_STATIC_DRAW);
// 	makeVAO(vbo);

// 	_initShaders(WHITE_SHADER);
// 	initProgram();
// 	glUseProgram(shaderProgram);
// 	glBindVertexArray(vao);
// 	// Drawing all the vertex of the triangle
// 	glDrawArrays(GL_LINE_LOOP, 0, 8);
// }

// void GameRenderer::createGrid(void) {
// 	int linesCount = 78;
// 	int pointsCount = linesCount * 2;
// 	float lineSpacing = squarePercentX;
// 	float tmpX = startX;
// 	float vertexBorders[pointsCount * 3];
// 	for (int i = 0; i < pointsCount * 3; i++) {
// 		vertexBorders[i] = 0;
// 	}

// 	// Vertical lines
// 	lineSpacing = squarePercentX;
// 	tmpX = startX + lineSpacing;
// 	for (int i = 0; i < linesCount / 2; i++) {
// 		// Start point
// 		int lineIdx = i * 6;
// 		vertexBorders[lineIdx] = tmpX;
// 		vertexBorders[lineIdx + 1] = startY;
// 		vertexBorders[lineIdx + 2] = 0.0f;

// 		// End point
// 		vertexBorders[lineIdx + 3] = tmpX;
// 		vertexBorders[lineIdx + 4] = -(startY);
// 		vertexBorders[lineIdx + 5] = 0.0f;

// 		tmpX += lineSpacing;
// 	}
// 	// Horizontal lines
// 	lineSpacing = squarePercentY;
// 	tmpX = startY - lineSpacing;
// 	for (int i = linesCount / 2; i < linesCount; i++) {
// 		// Start point
// 		int lineIdx = i * 6;
// 		vertexBorders[lineIdx] = startX;
// 		vertexBorders[lineIdx + 1] = tmpX;
// 		vertexBorders[lineIdx + 2] = 0.0f;

// 		// End point
// 		vertexBorders[lineIdx + 3] = -startX;
// 		vertexBorders[lineIdx + 4] = tmpX;
// 		vertexBorders[lineIdx + 5] = 0.0f;

// 		tmpX -= lineSpacing;
// 	}
// 	// BUFFER
// 	vbo = 0;
// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBorders), vertexBorders,
// 				 GL_STATIC_DRAW);
// 	makeVAO(vbo);

// 	_initShaders(WHITE_SHADER);
// 	initProgram();
// 	glUseProgram(shaderProgram);
// 	glBindVertexArray(vao);
// 	// Drawing all the vertex of the triangle
// 	glDrawArrays(GL_LINES, 0, pointsCount);
// }

// void GameRenderer::drawPlayer(Entity *player) {
// 	if (player == nullptr) return;
// 	float startCoordX = startX + (player->getPosition()[0] * squarePercentX);
// 	float startCoordY = startY - (player->getPosition()[2] * squarePercentY);

// 	float xCenter = startCoordX + (squarePercentX / 2);
// 	float yCenter = startCoordY - (squarePercentY / 2);
// 	int nbrOfSide = 120;
// 	float radiusX = squarePercentX / 2;
// 	float radiusY = squarePercentY / 2;

// 	GLint nbrOfVertices = nbrOfSide + 2;

// 	GLfloat doublePi = 2.0f * 3.14159265358979f;

// 	GLfloat circleVerticesX[nbrOfVertices];
// 	GLfloat circleVerticesY[nbrOfVertices];
// 	GLfloat circleVerticesZ[nbrOfVertices];

// 	circleVerticesX[0] = xCenter;
// 	circleVerticesY[0] = yCenter;
// 	circleVerticesZ[0] = 0;

// 	for (int i = 1; i < nbrOfVertices; i++) {
// 		circleVerticesX[i] =
// 			xCenter + (radiusX * cos(i * doublePi / nbrOfSide));
// 		circleVerticesY[i] =
// 			yCenter + (radiusY * sin(i * doublePi / nbrOfSide));
// 		circleVerticesZ[i] = 0;
// 	}
// 	GLfloat allCircleVertices[nbrOfVertices * 3];
// 	for (int i = 0; i < nbrOfVertices; i++) {
// 		allCircleVertices[i * 3] = circleVerticesX[i];
// 		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
// 		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
// 	}

// 	// BUFFER
// 	vbo = 0;
// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(allCircleVertices), allCircleVertices,
// 				 GL_STATIC_DRAW);
// 	makeVAO(vbo);

// 	_initShaders(GREEN_SHADER);
// 	initProgram();
// 	glUseProgram(shaderProgram);
// 	glBindVertexArray(vao);
// 	glDrawArrays(GL_TRIANGLE_FAN, 0, nbrOfVertices);
// }

// void GameRenderer::drawSquare(Entity *wall) {
// 	float startCoordX = startX + (wall->getPosition()[0] * squarePercentX);
// 	float startCoordY = startY - (wall->getPosition()[2] * squarePercentY);

// 	// POINTS
// 	float points[] = {startCoordX,
// 					  startCoordY,
// 					  0.0f,
// 					  startCoordX,
// 					  startCoordY - squarePercentY,
// 					  0.0f,
// 					  startCoordX + squarePercentX,
// 					  startCoordY,
// 					  0.0f,

// 					  startCoordX + squarePercentX,
// 					  startCoordY,
// 					  0.0f,
// 					  startCoordX + squarePercentX,
// 					  startCoordY - squarePercentY,
// 					  0.0f,
// 					  startCoordX,
// 					  startCoordY - squarePercentY,
// 					  0.0f};

// 	// BUFFER
// 	vbo = 0;
// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
// 	makeVAO(vbo);

// 	_initShaders(CYAN_SHADER);
// 	initProgram();
// 	glUseProgram(shaderProgram);
// 	glBindVertexArray(vao);
// 	// drawing all the vertex of the triangle
// 	glDrawArrays(GL_TRIANGLES, 0, 6);
// }

// void GameRenderer::initProgram(void) {
// 	shaderProgram = glCreateProgram();
// 	glAttachShader(shaderProgram, fs);
// 	glAttachShader(shaderProgram, vs);
// 	glLinkProgram(shaderProgram);
// }