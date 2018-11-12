#include "GameEngine.hpp"
#include "GameRenderer.hpp"

GameEngine::GameEngine() : _gameScenes(std::vector<AGameScene *>()) {}

GameEngine::GameEngine(std::vector<AGameScene *> gameScenes)
	: _gameScenes(gameScenes) {
	canRun = false;
	running = false;

	// Map size parsing
	mapW = MAP_SIZE;
	mapH = MAP_SIZE;
	int maxSizeW = (WINDOW_W - 2 * WINDOW_MIN_X_OFFSET) / mapW;
	int maxSizeH = (WINDOW_H - 2 * WINDOW_MIN_Y_OFFSET) / mapH;
	squareSize = (maxSizeH < maxSizeW) ? maxSizeH : maxSizeW;
	xOffset = (WINDOW_W - squareSize * mapW) / 2;
	yOffset = (WINDOW_H - squareSize * mapH) / 2;

	// Create interface class
	graphicLib = new GameRenderer(this);
	// Create audio manager
	audioManager = new AudioManager();

	// Force load of first scene
	_sceneIdx = 0;

	// Everything good
	canRun = true;
}

GameEngine::GameEngine(GameEngine const &src) { *this = src; }

GameEngine::~GameEngine(void) {
	delete (audioManager);
	delete (graphicLib);
}

int GameEngine::getSquareSize(void) { return squareSize; }

int GameEngine::getXOffset(void) { return xOffset; }

int GameEngine::getYOffset(void) { return yOffset; }

int GameEngine::getMapW(void) { return mapW; }

int GameEngine::getMapH(void) { return mapH; }

AEntity *GameEngine::getFirstEntityWithName(std::string entityName) {
	AEntity *foundElem = nullptr;
	for (auto entity : _activeEntities) {
		if (entity->name.compare(entityName) == 0) {
			foundElem = entity;
			break;
		}
	}
	return foundElem;
}

GameEngine &GameEngine::operator=(GameEngine const &rhs) {
	this->canRun = rhs.canRun;
	return *this;
}

bool GameEngine::initScene(int newSceneIdx) {
	if (newSceneIdx < 0 || newSceneIdx >= static_cast<int>(_gameScenes.size()))
		return false;
	_sceneIdx = newSceneIdx;
	_activeEntities.clear();
	for (auto entity : _gameScenes[newSceneIdx]->startEntities) {
		_activeEntities.push_back(entity);
		_activeEntities.back()->setGameEngine(this);
	}
	return true;
}

void GameEngine::checkCollisions(void) {
	for (size_t i = 0; i < _activeEntities.size() - 1; i++) {
		for (size_t j = i + 1; j < _activeEntities.size(); j++) {
			if (doCollide(_activeEntities[i], _activeEntities[j])) {
				// TODO: Handle collision
				std::cout << "Collision detected !" << std::endl;
			}
		}
	}
}

bool GameEngine::doCollide(AEntity *entityA, AEntity *entityB) {
	float aXCenter = entityA->position[0];
	float aYCenter = entityA->position[0];
	float bXCenter = entityB->position[2];
	float bYCenter = entityB->position[2];
	if (entityA->collider.shape == entityB->collider.shape) {
		// Circle with circle
		if (entityA->collider.shape == Collider::Circle) {
			float distance =
				sqrt(pow(aXCenter - bXCenter, 2) + pow(aYCenter - bYCenter, 2));
			return (distance <=
					entityA->collider.width + entityA->collider.width);
		}
		// Rectangle with rectangle
		else if (entityA->collider.shape == Collider::Rectangle) {
			return (abs(aXCenter - bXCenter) <=
						entityA->collider.width + entityA->collider.width &&
					abs(aYCenter - bYCenter) <=
						entityA->collider.height + entityA->collider.height);
		}
	}
	// Circle with rectangle
	else if (entityA->collider.shape == Collider::Circle &&
			 entityB->collider.shape == Collider::Rectangle) {
		return collisionCircleRectangle(entityA, entityB);
	}
	// Rectangle with circle
	else if (entityB->collider.shape == Collider::Circle &&
			 entityA->collider.shape == Collider::Rectangle) {
		return collisionCircleRectangle(entityB, entityA);
	}
	return false;
}

bool GameEngine::collisionCircleRectangle(AEntity *circleEntity,
										  AEntity *rectEntity) {
	float closestX = circleEntity->position[0];
	float closestY = circleEntity->position[2];
	// Find closest X of rectangle shape to circle center
	if (closestX > rectEntity->position[0] + rectEntity->collider.width)
		closestX = rectEntity->position[0] + rectEntity->collider.width;
	else if (closestX < rectEntity->position[0] - rectEntity->collider.width)
		closestX = rectEntity->position[0] - rectEntity->collider.width;
	// Find closest Y of rectangle shape to circle center
	if (closestY > rectEntity->position[2] + rectEntity->collider.height)
		closestY = rectEntity->position[2] + rectEntity->collider.height;
	else if (closestY < rectEntity->position[2] - rectEntity->collider.height)
		closestY = rectEntity->position[2] - rectEntity->collider.height;
	// Is distance of closer point smaller than circle radius ?
	return sqrt(pow(circleEntity->position[0] - closestX, 2) +
				pow(circleEntity->position[2] - closestY, 2)) <=
		   circleEntity->collider.width;
}

int GameEngine::run(void) {
	if (!canRun) return EXIT_FAILURE;
	int guiRet;

	// Init vars
	running = true;
	restartRequest = false;
	_lastFrameTs = Clock::now();

	if (!initScene(_sceneIdx)) std::runtime_error("Cannot load scene !");

	audioManager->playStartSound();

	// Start game loop
	while (running) {
		// Get delta time in order to synch entities positions
		_frameTs = Clock::now();
		_deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(
						  _frameTs - _lastFrameTs)
						  .count()) /
					 (double)1000.0;
		_lastFrameTs = _frameTs;

		// Update inputs
		graphicLib->getUserInput();

		// TODO: Update game engine statuses (ex. when to quit)
		if (running) {
			running = !keyboardMap["ESCAPE"];
		}

		// Update game entities states
		for (auto entity : _activeEntities) {
			entity->Update();
		}

		// TODO: set position of entities back to prev frame when they collide
		checkCollisions();

		guiRet = graphicLib->refreshWindow(_activeEntities);
		if (guiRet != EXIT_SUCCESS || !running) break;
	}
	if (restartRequest) {
		std::cout << "Starting new game!" << std::endl;
		return run();
	}
	return guiRet;
}

void GameEngine::buttonStateChanged(std::string buttonName, bool isPressed) {
	if (keyboardMap.find(buttonName) == keyboardMap.end()) {
		std::runtime_error("Unkown Mapping for '" + buttonName + "'!");
	}
	keyboardMap[buttonName] = isPressed;
}

bool GameEngine::isKeyPressed(std::string keyName) {
	return keyboardMap[keyName];
}

double GameEngine::getDeltaTime(void) { return _deltaTime; }

static std::map<std::string, bool>
generateKeyboardMap() {  // static here is "internal linkage"
	std::map<std::string, bool> map = std::map<std::string, bool>();
	map["UNKNOWN"] = false;
	map["SPACE"] = false;
	map["APOSTROPHE"] = false;
	map["COMMA"] = false;
	map["MINUS"] = false;
	map["PERIOD"] = false;
	map["SLASH"] = false;
	map["0"] = false;
	map["1"] = false;
	map["2"] = false;
	map["3"] = false;
	map["4"] = false;
	map["5"] = false;
	map["6"] = false;
	map["7"] = false;
	map["8"] = false;
	map["9"] = false;
	map[";"] = false;
	map["EQUAL"] = false;
	map["A"] = false;
	map["B"] = false;
	map["C"] = false;
	map["D"] = false;
	map["E"] = false;
	map["F"] = false;
	map["G"] = false;
	map["H"] = false;
	map["I"] = false;
	map["L"] = false;
	map["K"] = false;
	map["L"] = false;
	map["M"] = false;
	map["N"] = false;
	map["O"] = false;
	map["P"] = false;
	map["Q"] = false;
	map["R"] = false;
	map["S"] = false;
	map["T"] = false;
	map["U"] = false;
	map["V"] = false;
	map["W"] = false;
	map["X"] = false;
	map["Y"] = false;
	map["Z"] = false;
	map["["] = false;
	map["\\"] = false;
	map["]"] = false;
	map["`"] = false;
	map["WORLD_1"] = false;
	map["WORLD_2"] = false;
	map["ESCAPE"] = false;
	map["ENTER"] = false;
	map["TAB"] = false;
	map["BACKSPACE"] = false;
	map["INSERT"] = false;
	map["DELETE"] = false;
	map["RIGHT"] = false;
	map["LEFT"] = false;
	map["DOWN"] = false;
	map["UP"] = false;
	map["PAGE_UP"] = false;
	map["PAGE_DOWN"] = false;
	map["HOME"] = false;
	map["END"] = false;
	map["CAPS_LOCK"] = false;
	map["SCROLL_LOCK"] = false;
	map["NUM_LOCK"] = false;
	map["PRINT_SCREEN"] = false;
	map["PAUSE"] = false;
	map["F1"] = false;
	map["F2"] = false;
	map["F3"] = false;
	map["F4"] = false;
	map["F5"] = false;
	map["F6"] = false;
	map["F7"] = false;
	map["F8"] = false;
	map["F9"] = false;
	map["F10"] = false;
	map["F11"] = false;
	map["F12"] = false;
	map["F13"] = false;
	map["F14"] = false;
	map["F15"] = false;
	map["F16"] = false;
	map["F17"] = false;
	map["F18"] = false;
	map["F19"] = false;
	map["F20"] = false;
	map["F21"] = false;
	map["F22"] = false;
	map["F23"] = false;
	map["F24"] = false;
	map["F25"] = false;
	map["KP_0"] = false;
	map["KP_1"] = false;
	map["KP_2"] = false;
	map["KP_3"] = false;
	map["KP_4"] = false;
	map["KP_5"] = false;
	map["KP_6"] = false;
	map["KP_7"] = false;
	map["KP_8"] = false;
	map["KP_9"] = false;
	map["KP_DECIMAL"] = false;
	map["KP_DIVIDE"] = false;
	map["KP_MULTIPLY"] = false;
	map["KP_SUBTRACT"] = false;
	map["KP_ADD"] = false;
	map["KP_ENTER"] = false;
	map["KP_EQUAL"] = false;
	map["LEFT_SHIFT"] = false;
	map["LEFT_CONTROL"] = false;
	map["LEFT_ALT"] = false;
	map["LEFT_SUPER"] = false;
	map["RIGHT_SHIFT"] = false;
	map["RIGHT_CONTROL"] = false;
	map["RIGHT_ALT"] = false;
	map["RIGHT_SUPER"] = false;
	map["MENU"] = false;
	return map;
}
std::map<std::string, bool> GameEngine::keyboardMap = generateKeyboardMap();
