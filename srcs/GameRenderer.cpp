#include "GameRenderer.hpp"
#include "GameLogic.hpp"

GameRenderer::GameRenderer(GameLogic *_mainGame)
{
	this->mainGame = _mainGame;
	// std::cout << "GLFW window" << std::endl;
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
		throw new std::runtime_error("Failed to initialize GLFW");
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	this->window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Bomberman", NULL, NULL); // Size of screen will change
	if (!this->window)
	{
		glfwTerminate();
		throw new std::runtime_error("Failed to create windows GLFW");
	}
	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width / 2) - (WINDOW_W / 2), (mode->height / 2) - (WINDOW_H / 2));
	glfwMakeContextCurrent(this->window);
	glfwGetWindowSize(window, &width, &height);
	glfwSetWindowUserPointer(window, this);
	// graphicUI = new GUI(window);
	// glViewport(0, 0, WINDOW_W, WINDOW_H);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw new std::runtime_error("Failed to initialize GLAD");

	glfwSetKeyCallback(window, keyCallback);
	glfwPollEvents();

	squareSize = mainGame->getSquareSize();
	xOffset = mainGame->getXOffset();
	yOffset = mainGame->getYOffset();

	// Get top left of game screen
	startX = -((WINDOW_W / 2.0f) - xOffset) / (WINDOW_W / 2.0f);
	startY = ((WINDOW_H / 2.0f) - yOffset) / (WINDOW_H / 2.0f);

	// Get the size of each square in the game screen
	squarePercentY = startY / (_mainGame->getMapH() / 2.0f);
	squarePercentX = (-startX) / (_mainGame->getMapW() / 2.0f);

	graphicUI = new GUI(window);
	// std::cout << "6" << std::endl;

	return;
}

GameRenderer::GameRenderer(void)
{
	return;
}

GameRenderer::GameRenderer(GameRenderer const &src)
{
	*this = src;
	return;
}

GameRenderer::~GameRenderer(void)
{
	std::cout << "toto" << std::endl;
	delete graphicUI;
	std::cout << "toto1" << std::endl;
	closeWindow();
	std::cout << "toto2" << std::endl;
	return;
}

GameRenderer &GameRenderer::operator=(GameRenderer const &rhs)
{
	this->active = rhs.active;
	return *this;
}

void GameRenderer::makeVAO(GLuint &vbo)
{
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void GameRenderer::initShaders(int type)
{
	// Shader pour les vertex
	vertexShader =
		"#version 400\n"
		"in vec3 vp;"
		"void main() {"
		"  gl_Position = vec4(vp, 1.0);"
		"}";

	if (type == WHITE_SHADER)
	{
		// Shader pour dessiner ce qu'il y a entre les vertex
		fragmentShader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);"
			"}";
	}
	else if (type == CYAN_SHADER)
	{
		// Shader pour dessiner ce qu'il y a entre les vertex
		fragmentShader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(0.0, 0.9, 0.7, 1.0);"
			"}";
	}
	else if (type == RED_SHADER)
	{
		// Shader pour dessiner ce qu'il y a entre les vertex
		fragmentShader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
			"}";
	}
	else if (type == GREEN_SHADER)
	{
		// Shader pour dessiner ce qu'il y a entre les vertex
		fragmentShader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(0.0, 0.9, 0.0, 1.0);"
			"}";
	}
	else if (type == YELLOW_SHADER)
	{
		// Shader pour dessiner ce qu'il y a entre les vertex
		fragmentShader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(1.0, 1.0, 0.0, 1.0);"
			"}";
	}
	else if (type == GRAY_SHADER)
	{
		// Shader pour dessiner ce qu'il y a entre les vertex
		fragmentShader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(185.0/255.0, 185.0/255.0, 146.0/255.0, 1.0);"
			"}";
	}

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShader, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShader, NULL);
	glCompileShader(fs);

	//check if shaders compile fail
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
		// std::cout << "Vertex Shader compile failed" << std::endl;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
		std::cout << "Fragment Shader compile failed" << std::endl;
}

void GameRenderer::initProgram(void)
{
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fs);
	glAttachShader(shaderProgram, vs);
	glLinkProgram(shaderProgram);
}

void GameRenderer::createBorder(void)
{
	float epsilonX = 1 / (WINDOW_W / 2.0f);
	float epsilonY = 1 / (WINDOW_H / 2.0f);

	float vertexBorders[] =
		{
			startX, startY + epsilonY, 0.0f,			   // top-left
			-(startX) + epsilonX, startY + epsilonY, 0.0f, // top-right

			-(startX) + epsilonX, startY + epsilonY, 0.0f, // top-right
			-(startX) + epsilonX, -(startY), 0.0f,		   // bottom-right

			-(startX) + epsilonX, -(startY), 0.0f, // bottom-right
			startX, -(startY), 0.0f,			   // bottom-left

			startX, -(startY), 0.0f,		// bottom-left
			startX, startY + epsilonY, 0.0f // top-left
		};
	//BUFFER
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBorders), vertexBorders, GL_STATIC_DRAW);
	makeVAO(vbo);

	initShaders(1);
	initProgram();
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	//drawing all the vertex of the triangle
	glDrawArrays(GL_LINE_LOOP, 0, 8);
}

void GameRenderer::createGrid(void)
{
	int linesCount = 78;
	int pointsCount = linesCount * 2;
	float lineSpacing = squarePercentX;
	float tmpX = startX;
	float vertexBorders[pointsCount * 3];
	for (int i = 0; i < pointsCount * 3; i++)
	{
		vertexBorders[i] = 0;
	}

	// Vertical lines
	lineSpacing = squarePercentX;
	tmpX = startX + lineSpacing;
	for (int i = 0; i < linesCount / 2; i++)
	{
		// Start point
		int lineIdx = i * 6;
		vertexBorders[lineIdx] = tmpX;
		vertexBorders[lineIdx + 1] = startY;
		vertexBorders[lineIdx + 2] = 0.0f;

		// End point
		vertexBorders[lineIdx + 3] = tmpX;
		vertexBorders[lineIdx + 4] = -(startY);
		vertexBorders[lineIdx + 5] = 0.0f;

		tmpX += lineSpacing;
	}
	// Horizontal lines
	lineSpacing = squarePercentY;
	tmpX = startY - lineSpacing;
	for (int i = linesCount / 2; i < linesCount; i++)
	{
		// Start point
		int lineIdx = i * 6;
		vertexBorders[lineIdx] = startX;
		vertexBorders[lineIdx + 1] = tmpX;
		vertexBorders[lineIdx + 2] = 0.0f;

		// End point
		vertexBorders[lineIdx + 3] = -startX;
		vertexBorders[lineIdx + 4] = tmpX;
		vertexBorders[lineIdx + 5] = 0.0f;

		tmpX -= lineSpacing;
	}
	//BUFFER
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBorders), vertexBorders, GL_STATIC_DRAW);
	makeVAO(vbo);

	initShaders(1);
	initProgram();
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	//drawing all the vertex of the triangle
	glDrawArrays(GL_LINES, 0, pointsCount);
}

void GameRenderer::drawPlayer(std::tuple<int, int> &playerPos)
{
	float startCoordX = startX + (std::get<0>(playerPos) * squarePercentX);
	float startCoordY = startY - (std::get<1>(playerPos) * squarePercentY);

	float xCenter = startCoordX + (squarePercentX / 2);
	float yCenter = startCoordY - (squarePercentY / 2);
	int nbrOfSide = 120;
	float radiusX = squarePercentX / 2;
	float radiusY = squarePercentY / 2;

	GLint nbrOfVertices = nbrOfSide + 2;

	GLfloat doublePi = 2.0f * 3.14159265358979f;

	GLfloat circleVerticesX[nbrOfVertices];
	GLfloat circleVerticesY[nbrOfVertices];
	GLfloat circleVerticesZ[nbrOfVertices];

	circleVerticesX[0] = xCenter;
	circleVerticesY[0] = yCenter;
	circleVerticesZ[0] = 0;

	for (int i = 1; i < nbrOfVertices; i++)
	{
		circleVerticesX[i] = xCenter + (radiusX * cos(i * doublePi / nbrOfSide));
		circleVerticesY[i] = yCenter + (radiusY * sin(i * doublePi / nbrOfSide));
		circleVerticesZ[i] = 0;
	}
	GLfloat allCircleVertices[nbrOfVertices * 3];
	for (int i = 0; i < nbrOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	//BUFFER
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(allCircleVertices), allCircleVertices, GL_STATIC_DRAW);
	makeVAO(vbo);

	initShaders(GREEN_SHADER);
	initProgram();
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, nbrOfVertices);
}

void GameRenderer::getUserInput(void)
{
	glfwPollEvents();
}

void GameRenderer::refreshWindow(void)
{
	// std::cout << "7" << std::endl;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// std::cout << "8" << std::endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// std::cout << "9" << std::endl;
	createBorder();
	// std::cout << "10" << std::endl;
	drawPlayer(mainGame->getPlayerPos());
	// std::cout << "11" << std::endl;
	createGrid();
	// std::cout << "12" << std::endl;
	graphicUI->drawGUI();
	// std::cout << "13" << std::endl;
	//put everything to screen
	glfwSwapBuffers(this->window);
}

void GameRenderer::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_UNKNOWN:
			mainGame->buttonStateChanged("UNKNOWN", action == GLFW_PRESS);
			break;

		case GLFW_KEY_SPACE:
			mainGame->buttonStateChanged("SPACE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_APOSTROPHE:
			mainGame->buttonStateChanged("APOSTROPHE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_COMMA:
			mainGame->buttonStateChanged("COMMA", action == GLFW_PRESS);
			break;

		case GLFW_KEY_MINUS:
			mainGame->buttonStateChanged("MINUS", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PERIOD:
			mainGame->buttonStateChanged("PERIOD", action == GLFW_PRESS);
			break;

		case GLFW_KEY_SLASH:
			mainGame->buttonStateChanged("SLASH", action == GLFW_PRESS);
			break;

		case GLFW_KEY_0:
			mainGame->buttonStateChanged("0", action == GLFW_PRESS);
			break;

		case GLFW_KEY_1:
			mainGame->buttonStateChanged("1", action == GLFW_PRESS);
			break;

		case GLFW_KEY_2:
			mainGame->buttonStateChanged("2", action == GLFW_PRESS);
			break;

		case GLFW_KEY_3:
			mainGame->buttonStateChanged("3", action == GLFW_PRESS);
			break;

		case GLFW_KEY_4:
			mainGame->buttonStateChanged("4", action == GLFW_PRESS);
			break;

		case GLFW_KEY_5:
			mainGame->buttonStateChanged("5", action == GLFW_PRESS);
			break;

		case GLFW_KEY_6:
			mainGame->buttonStateChanged("6", action == GLFW_PRESS);
			break;

		case GLFW_KEY_7:
			mainGame->buttonStateChanged("7", action == GLFW_PRESS);
			break;

		case GLFW_KEY_8:
			mainGame->buttonStateChanged("8", action == GLFW_PRESS);
			break;

		case GLFW_KEY_9:
			mainGame->buttonStateChanged("9", action == GLFW_PRESS);
			break;

		case GLFW_KEY_SEMICOLON:
			mainGame->buttonStateChanged(";", action == GLFW_PRESS);
			break;

		case GLFW_KEY_EQUAL:
			mainGame->buttonStateChanged("EQUAL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_A:
			mainGame->buttonStateChanged("A", action == GLFW_PRESS);
			break;

		case GLFW_KEY_B:
			mainGame->buttonStateChanged("B", action == GLFW_PRESS);
			break;

		case GLFW_KEY_C:
			mainGame->buttonStateChanged("C", action == GLFW_PRESS);
			break;

		case GLFW_KEY_D:
			mainGame->buttonStateChanged("D", action == GLFW_PRESS);
			break;

		case GLFW_KEY_E:
			mainGame->buttonStateChanged("E", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F:
			mainGame->buttonStateChanged("F", action == GLFW_PRESS);
			break;

		case GLFW_KEY_G:
			mainGame->buttonStateChanged("G", action == GLFW_PRESS);
			break;

		case GLFW_KEY_H:
			mainGame->buttonStateChanged("H", action == GLFW_PRESS);
			break;

		case GLFW_KEY_I:
			mainGame->buttonStateChanged("I", action == GLFW_PRESS);
			break;

		case GLFW_KEY_J:
			mainGame->buttonStateChanged("L", action == GLFW_PRESS);
			break;

		case GLFW_KEY_K:
			mainGame->buttonStateChanged("K", action == GLFW_PRESS);
			break;

		case GLFW_KEY_L:
			mainGame->buttonStateChanged("L", action == GLFW_PRESS);
			break;

		case GLFW_KEY_M:
			mainGame->buttonStateChanged("M", action == GLFW_PRESS);
			break;

		case GLFW_KEY_N:
			mainGame->buttonStateChanged("N", action == GLFW_PRESS);
			break;

		case GLFW_KEY_O:
			mainGame->buttonStateChanged("O", action == GLFW_PRESS);
			break;

		case GLFW_KEY_P:
			mainGame->buttonStateChanged("P", action == GLFW_PRESS);
			break;

		case GLFW_KEY_Q:
			mainGame->buttonStateChanged("Q", action == GLFW_PRESS);
			break;

		case GLFW_KEY_R:
			mainGame->buttonStateChanged("R", action == GLFW_PRESS);
			break;

		case GLFW_KEY_S:
			mainGame->buttonStateChanged("S", action == GLFW_PRESS);
			break;

		case GLFW_KEY_T:
			mainGame->buttonStateChanged("T", action == GLFW_PRESS);
			break;

		case GLFW_KEY_U:
			mainGame->buttonStateChanged("U", action == GLFW_PRESS);
			break;

		case GLFW_KEY_V:
			mainGame->buttonStateChanged("V", action == GLFW_PRESS);
			break;

		case GLFW_KEY_W:
			mainGame->buttonStateChanged("W", action == GLFW_PRESS);
			break;

		case GLFW_KEY_X:
			mainGame->buttonStateChanged("X", action == GLFW_PRESS);
			break;

		case GLFW_KEY_Y:
			mainGame->buttonStateChanged("Y", action == GLFW_PRESS);
			break;

		case GLFW_KEY_Z:
			mainGame->buttonStateChanged("Z", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_BRACKET:
			mainGame->buttonStateChanged("[", action == GLFW_PRESS);
			break;

		case GLFW_KEY_BACKSLASH:
			mainGame->buttonStateChanged("\\", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_BRACKET:
			mainGame->buttonStateChanged("]", action == GLFW_PRESS);
			break;

		case GLFW_KEY_GRAVE_ACCENT:
			mainGame->buttonStateChanged("`", action == GLFW_PRESS);
			break;

		case GLFW_KEY_WORLD_1:
			mainGame->buttonStateChanged("WORLD_1", action == GLFW_PRESS);
			break;

		case GLFW_KEY_WORLD_2:
			mainGame->buttonStateChanged("WORLD_2", action == GLFW_PRESS);
			break;

		case GLFW_KEY_ESCAPE:
			mainGame->buttonStateChanged("ESCAPE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_ENTER:
			mainGame->buttonStateChanged("ENTER", action == GLFW_PRESS);
			break;

		case GLFW_KEY_TAB:
			mainGame->buttonStateChanged("TAB", action == GLFW_PRESS);
			break;

		case GLFW_KEY_BACKSPACE:
			mainGame->buttonStateChanged("BACKSPACE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_INSERT:
			mainGame->buttonStateChanged("INSERT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_DELETE:
			mainGame->buttonStateChanged("DELETE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT:
			mainGame->buttonStateChanged("RIGHT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT:
			mainGame->buttonStateChanged("LEFT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_DOWN:
			mainGame->buttonStateChanged("DOWN", action == GLFW_PRESS);
			break;

		case GLFW_KEY_UP:
			mainGame->buttonStateChanged("UP", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PAGE_UP:
			mainGame->buttonStateChanged("PAGE_UP", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PAGE_DOWN:
			mainGame->buttonStateChanged("PAGE_DOWN", action == GLFW_PRESS);
			break;

		case GLFW_KEY_HOME:
			mainGame->buttonStateChanged("HOME", action == GLFW_PRESS);
			break;

		case GLFW_KEY_END:
			mainGame->buttonStateChanged("END", action == GLFW_PRESS);
			break;

		case GLFW_KEY_CAPS_LOCK:
			mainGame->buttonStateChanged("CAPS_LOCK", action == GLFW_PRESS);
			break;

		case GLFW_KEY_SCROLL_LOCK:
			mainGame->buttonStateChanged("SCROLL_LOCK", action == GLFW_PRESS);
			break;

		case GLFW_KEY_NUM_LOCK:
			mainGame->buttonStateChanged("NUM_LOCK", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PRINT_SCREEN:
			mainGame->buttonStateChanged("PRINT_SCREEN", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PAUSE:
			mainGame->buttonStateChanged("PAUSE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F1:
			mainGame->buttonStateChanged("F1", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F2:
			mainGame->buttonStateChanged("F2", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F3:
			mainGame->buttonStateChanged("F3", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F4:
			mainGame->buttonStateChanged("F4", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F5:
			mainGame->buttonStateChanged("F5", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F6:
			mainGame->buttonStateChanged("F6", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F7:
			mainGame->buttonStateChanged("F7", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F8:
			mainGame->buttonStateChanged("F8", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F9:
			mainGame->buttonStateChanged("F9", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F10:
			mainGame->buttonStateChanged("F10", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F11:
			mainGame->buttonStateChanged("F11", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F12:
			mainGame->buttonStateChanged("F12", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F13:
			mainGame->buttonStateChanged("F13", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F14:
			mainGame->buttonStateChanged("F14", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F15:
			mainGame->buttonStateChanged("F15", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F16:
			mainGame->buttonStateChanged("F16", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F17:
			mainGame->buttonStateChanged("F17", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F18:
			mainGame->buttonStateChanged("F18", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F19:
			mainGame->buttonStateChanged("F19", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F20:
			mainGame->buttonStateChanged("F20", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F21:
			mainGame->buttonStateChanged("F21", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F22:
			mainGame->buttonStateChanged("F22", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F23:
			mainGame->buttonStateChanged("F23", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F24:
			mainGame->buttonStateChanged("F24", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F25:
			mainGame->buttonStateChanged("F25", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_0:
			mainGame->buttonStateChanged("KP_0", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_1:
			mainGame->buttonStateChanged("KP_1", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_2:
			mainGame->buttonStateChanged("KP_2", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_3:
			mainGame->buttonStateChanged("KP_3", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_4:
			mainGame->buttonStateChanged("KP_4", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_5:
			mainGame->buttonStateChanged("KP_5", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_6:
			mainGame->buttonStateChanged("KP_6", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_7:
			mainGame->buttonStateChanged("KP_7", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_8:
			mainGame->buttonStateChanged("KP_8", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_9:
			mainGame->buttonStateChanged("KP_9", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_DECIMAL:
			mainGame->buttonStateChanged("KP_DECIMAL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_DIVIDE:
			mainGame->buttonStateChanged("KP_DIVIDE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_MULTIPLY:
			mainGame->buttonStateChanged("KP_MULTIPLY", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_SUBTRACT:
			mainGame->buttonStateChanged("KP_SUBTRACT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_ADD:
			mainGame->buttonStateChanged("KP_ADD", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_ENTER:
			mainGame->buttonStateChanged("KP_ENTER", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_EQUAL:
			mainGame->buttonStateChanged("KP_EQUAL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_SHIFT:
			mainGame->buttonStateChanged("LEFT_SHIFT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_CONTROL:
			mainGame->buttonStateChanged("LEFT_CONTROL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_ALT:
			mainGame->buttonStateChanged("LEFT_ALT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_SUPER:
			mainGame->buttonStateChanged("LEFT_SUPER", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_SHIFT:
			mainGame->buttonStateChanged("RIGHT_SHIFT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_CONTROL:
			mainGame->buttonStateChanged("RIGHT_CONTROL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_ALT:
			mainGame->buttonStateChanged("RIGHT_ALT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_SUPER:
			mainGame->buttonStateChanged("RIGHT_SUPER", action == GLFW_PRESS);
			break;

		case GLFW_KEY_MENU:
			mainGame->buttonStateChanged("MENU", action == GLFW_PRESS);
			break;
		default:
			break;
		}
	}
	(void)scancode;
	(void)window;
	(void)mods;
}

void GameRenderer::closeWindow()
{
	// std::cout << "Destroing Glfw window" << std::endl;
	std::cout << "toto pas content" << std::endl;
	if (window)
		glfwDestroyWindow(this->window);
	std::cout << "toto pas content2" << std::endl;
	glfwTerminate();
}

void GameRenderer::errorCallback(int error, const char *description)
{
	std::cerr << "Error n." << error << ": " << description << std::endl;
}

GameLogic *GameRenderer::mainGame = NULL;
