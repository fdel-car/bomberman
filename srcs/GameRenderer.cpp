/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameRenderer.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jichen-m <jichen-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 19:46:10 by jichen-m          #+#    #+#             */
/*   Updated: 2018/11/08 13:28:15 by jichen-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
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

	// Nuklear init
	ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
	struct nk_font_atlas *atlas;
	nk_glfw3_font_stash_begin(&atlas);
	nk_glfw3_font_stash_end();

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
	closeWindow();
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
		std::cout << "Vertex Shader compile failed" << std::endl;
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

void GameRenderer::drawGUI(void)
{
	nk_glfw3_new_frame();

	/* GUI */
	if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
				 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
					 NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		enum
		{
			EASY,
			HARD
		};
		static int op = EASY;
		static int property = 20;
		nk_layout_row_static(ctx, 30, 80, 1);
		if (nk_button_label(ctx, "button"))
			fprintf(stdout, "button pressed\n");

		nk_layout_row_dynamic(ctx, 30, 2);
		if (nk_option_label(ctx, "easy", op == EASY))
			op = EASY;
		if (nk_option_label(ctx, "hard", op == HARD))
			op = HARD;

		nk_layout_row_dynamic(ctx, 25, 1);
		nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "background:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400)))
		{
			nk_layout_row_dynamic(ctx, 120, 1);
			bg = nk_color_picker(ctx, bg, NK_RGBA);
			nk_layout_row_dynamic(ctx, 25, 1);
			bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
			bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
			bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
			bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
			nk_combo_end(ctx);
		}
	}
	nk_end(ctx);
	nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	createBorder();
	drawPlayer(mainGame->getPlayerPos());

	createGrid();

	drawGUI();

	//put everything to screen
	glfwSwapBuffers(this->window);
}

void GameRenderer::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		mainGame->buttonStateChanged(glfwGetKeyName(key, scancode), action == GLFW_PRESS);
		// mainGame->buttonStateChanged("NULL", action == GLFW_PRESS);
		// switch (key)
		// {
		// case GLFW_KEY_RIGHT:
		// 	mainGame->buttonPressed(KEY_RIGHT);
		// 	break;
		// case GLFW_KEY_LEFT:
		// 	mainGame->buttonPressed(KEY_LEFT);
		// 	break;
		// case GLFW_KEY_UP:
		// 	mainGame->buttonPressed(KEY_UP);
		// 	break;
		// case GLFW_KEY_DOWN:
		// 	mainGame->buttonPressed(KEY_DOWN);
		// 	break;
		// default:
		// 	mainGame->buttonPressed(glfwGetKeyName(key, scancode));
		// 	break;
		// }
	}
	(void)key;
	(void)scancode;
	(void)action;
	(void)window;
	(void)mods;
}

void GameRenderer::closeWindow()
{
	// std::cout << "Destroing Glfw window" << std::endl;
	if (window)
		glfwDestroyWindow(this->window);
	glfwTerminate();
}

void GameRenderer::errorCallback(int error, const char *description)
{
	std::cerr << "Error n." << error << ": " << description << std::endl;
}

GameLogic *GameRenderer::mainGame = NULL;
