#include "GameEngine.hpp"
#include "GameRenderer.hpp"

GameEngine::GameEngine() : _gameScenes(std::vector<AGameScene *>()) {}

GameEngine::GameEngine(std::vector<AGameScene *> gameScenes)
	: _gameScenes(gameScenes) {
	_running = false;

	// // Map size parsing
	// mapW = MAP_SIZE;
	// mapH = MAP_SIZE;
	// int maxSizeW = (WINDOW_W - 2 * WINDOW_MIN_X_OFFSET) / mapW;
	// int maxSizeH = (WINDOW_H - 2 * WINDOW_MIN_Y_OFFSET) / mapH;
	// squareSize = (maxSizeH < maxSizeW) ? maxSizeH : maxSizeW;
	// xOffset = (WINDOW_W - squareSize * mapW) / 2;
	// yOffset = (WINDOW_H - squareSize * mapH) / 2;

	// Create interface class
	_gameRenderer = new GameRenderer(this);
	// Create audio manager
	_audioManager = new AudioManager();

	// Force load of first scene
	_sceneIdx = 0;
}

GameEngine::~GameEngine(void) {
	delete _audioManager;
	delete _gameRenderer;
}

int GameEngine::getSquareSize(void) { return squareSize; }

int GameEngine::getXOffset(void) { return xOffset; }

int GameEngine::getYOffset(void) { return yOffset; }

int GameEngine::getMapW(void) { return mapW; }

int GameEngine::getMapH(void) { return mapH; }

// Entity *GameEngine::getFirstEntityWithName(std::string entityName) {
// 	Entity *foundElem = nullptr;
// 	for (auto entity : _allEntities) {
// 		if (entity->getName().compare(entityName) == 0) {
// 			foundElem = entity;
// 			break;
// 		}
// 	}
// 	return foundElem;
// }

bool GameEngine::initScene(int newSceneIdx) {
	if (newSceneIdx < 0 || newSceneIdx >= static_cast<int>(_gameScenes.size()))
		return false;
	_sceneIdx = newSceneIdx;
	_clearTmpEntities();
	_gameScenes[newSceneIdx]->load();
	for (auto data : _gameScenes[newSceneIdx]->getData()) {
		_allEntities.push_back(
			new Entity(data.pos, data.eulerAngles, data.collider,
					   (_gameRenderer->getShapes())[data.shapeName], true));
		_allEntities.back()->setGameEngine(this);
	}
	return true;
}

void GameEngine::_clearTmpEntities(void) {
	size_t idx = 0;
	for (auto entity : _allEntities) {
		if (entity->getTmpState())
			_allEntities.erase(_allEntities.begin() + idx);
		else
			idx++;
	}
}

// void GameEngine::checkCollisions(void) {
// 	for (size_t i = 0; i < _allEntities.size() - 1; i++) {
// 		for (size_t j = i + 1; j < _allEntities.size(); j++) {
// 			if (doCollide(_allEntities[i], _allEntities[j])) {
// 				// TODO: Handle collision
// 				std::cout << "Collision detected !" << std::endl;
// 			}
// 		}
// 	}
// }

// bool GameEngine::doCollide(Entity *entityA, Entity *entityB) {
// 	const Collider *colliderA = entityA->getCollider();
// 	const Collider *colliderB = entityB->getCollider();
// 	if (!colliderA || !colliderB) return false;

// 	float aXCenter = entityA->getPosition()[0];
// 	float aYCenter = entityA->getPosition()[0];
// 	float bXCenter = entityB->getPosition()[2];
// 	float bYCenter = entityB->getPosition()[2];
// 	if (colliderA->shape == colliderB->shape) {
// 		// Circle with circle
// 		if (colliderA->shape == Collider::Circle) {
// 			float distance =
// 				sqrt(pow(aXCenter - bXCenter, 2) + pow(aYCenter - bYCenter, 2));
// 			return (distance <= colliderA->width + colliderB->width);
// 		}
// 		// Rectangle with rectangle
// 		else if (colliderA->shape == Collider::Rectangle) {
// 			return (abs(aXCenter - bXCenter) <=
// 						colliderA->width + colliderB->width &&
// 					abs(aYCenter - bYCenter) <=
// 						colliderA->height + colliderB->height);
// 		}
// 	}
// 	// Circle with rectangle
// 	else if (colliderA->shape == Collider::Circle &&
// 			 colliderB->shape == Collider::Rectangle) {
// 		return collisionCircleRectangle(entityA, entityB);
// 	}
// 	// Rectangle with circle
// 	else if (colliderB->shape == Collider::Circle &&
// 			 colliderA->shape == Collider::Rectangle) {
// 		return collisionCircleRectangle(entityB, entityA);
// 	}
// 	return false;
// }

// bool GameEngine::collisionCircleRectangle(Entity *circleEntity,
// 										  Entity *rectEntity) {
// 	float closestX = circleEntity->getPosition()[0];
// 	float closestY = circleEntity->getPosition()[2];
// 	// Find closest X of rectangle shape to circle center
// 	if (closestX >
// 		rectEntity->getPosition()[0] + rectEntity->getCollider()->width)
// 		closestX =
// 			rectEntity->getPosition()[0] + rectEntity->getCollider()->width;
// 	else if (closestX <
// 			 rectEntity->getPosition()[0] - rectEntity->getCollider()->width)
// 		closestX =
// 			rectEntity->getPosition()[0] - rectEntity->getCollider()->width;
// 	// Find closest Y of rectangle shape to circle center
// 	if (closestY >
// 		rectEntity->getPosition()[2] + rectEntity->getCollider()->height)
// 		closestY =
// 			rectEntity->getPosition()[2] + rectEntity->getCollider()->height;
// 	else if (closestY <
// 			 rectEntity->getPosition()[2] - rectEntity->getCollider()->height)
// 		closestY =
// 			rectEntity->getPosition()[2] - rectEntity->getCollider()->height;
// 	// Is distance of closer point smaller than circle radius ?
// 	return sqrt(pow(circleEntity->getPosition()[0] - closestX, 2) +
// 				pow(circleEntity->getPosition()[2] - closestY, 2)) <=
// 		   circleEntity->getCollider()->width;
// }

void GameEngine::run(void) {
	// Init vars
	_running = true;
	restartRequest = false;
	_lastFrameTs = Clock::now();

	if (!initScene(_sceneIdx)) std::runtime_error("Cannot load scene !");

	_audioManager->playStartSound();

	// Start game loop
	while (_running) {
		// Get delta time in order to synch entities positions
		_frameTs = Clock::now();
		_deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(
						  _frameTs - _lastFrameTs)
						  .count()) /
					 static_cast<double>(1000.0);
		_lastFrameTs = _frameTs;

		// Update inputs
		_gameRenderer->getUserInput();

		// TODO: Update game engine statuses (ex. when to quit)
		if (_running) {
			_running = !keyboardMap["ESCAPE"];
		}

		// Update game entities states
		for (auto entity : _allEntities) {
			entity->Update();
		}

		// TODO: set position of entities back to prev frame when they collide
		// checkCollisions();

		_gameRenderer->refreshWindow(_allEntities);
	}
	if (restartRequest) {
		std::cout << "Starting new game!" << std::endl;
		return run();
	}
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
