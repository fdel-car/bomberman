#include "GameEngine.hpp"
#include "GameRenderer.hpp"

GameEngine::GameEngine() : _gameScenes(std::vector<AGameScene *>()) {}

GameEngine::LineInfo::LineInfo(void)
	: m(0.0f),
	  q(0.0f),
	  isVertical(false),
	  startX(0.0f),
	  startZ(0.0f),
	  endX(0.0f),
	  endZ(0.0f) {}

GameEngine::LineInfo::LineInfo(float startX, float startZ, float endX,
							   float endZ)
	: startX(startX), startZ(startZ), endX(endX), endZ(endZ) {
	isVertical = startX == endX;
	if (!isVertical) {
		m = (startZ - endZ) / (startX - endX);
		q = startZ - m * startX;
	}
}

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

// Entity *GameEngine::getFirstEntityWithName(std::string entityName) {
// 	Entity *foundElem = nullptr;
// 	for (auto entity : _activeEntities) {
// 		if (entity->getName().compare(entityName) == 0) {
// 			foundElem = entity;
// 			break;
// 		}
// 	}
// 	return foundElem;
// }

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

void GameEngine::moveEntities(void) {
	const Collider *collider;
	bool collisionDetected = false;
	std::vector<float> futurePos = std::vector<float>(3, 0);
	LineInfo lineA;
	LineInfo lineB;

	for (size_t i = 0; i < _activeEntities.size(); i++) {
		collisionDetected = false;
		if (_activeEntities[i]->getTargetMovement()[0] != 0.0f ||
			_activeEntities[i]->getTargetMovement()[2] != 0.0f) {
			collider = _activeEntities[i]->getCollider();

			if (collider) {
				// 1) Find lines that inglobe movement
				getMovementLines(_activeEntities[i], &lineA, &lineB);
				// If lines are vertical, create Rectangle Collider only once
				Collider moveCollider(Collider::Rectangle, 0.0f, 0.0f);
				std::vector<float> centerPos(3, 0);
				if (lineA.isVertical) {
					moveCollider.width =
						abs(lineA.startX - lineB.startX) / 2.0f;
					moveCollider.height = abs(lineA.startZ - lineA.endZ) / 2.0f;
					centerPos[0] = (lineA.startX + lineB.startX) / 2;
					centerPos[2] = (lineA.startZ + lineA.endZ) / 2;
				}

				// Simulate movement
				futurePos[0] = _activeEntities[i]->getPosition()[0] +
							   _activeEntities[i]->getTargetMovement()[0];
				futurePos[2] = _activeEntities[i]->getPosition()[2] +
							   _activeEntities[i]->getTargetMovement()[2];

				for (size_t j = 0; j < _activeEntities.size(); j++) {
					if (j == i) continue;
					// TODO: skip collision with object that cannot collide with

					// 2) Check if final position collides with smth
					// 3) Check if any other obj collides with lineA/lineB
					if (doCollide(collider, futurePos, _activeEntities[j]) ||
						(lineA.isVertical && doCollide(&moveCollider, centerPos,
													   _activeEntities[j])) ||
						(!lineA.isVertical &&
						 hasCollisionCourse(lineA, lineB,
											_activeEntities[j]))) {
						// TODO: handle if is physical collision or trigger
						collisionDetected = true;
						break;
					}
				}

				// If no collision, everything is good
				if (!collisionDetected) {
					_activeEntities[i]->moveFromPosition(
						_activeEntities[i]->getTargetMovement());
				}
				// TODO: Else
				else {
					// TODO
					std::cout << "Collision detected !" << std::endl;
				}
			} else {
				// Skip checks if entity doesnt have a collider
				_activeEntities[i]->moveFromPosition(
					_activeEntities[i]->getTargetMovement());
			}
		}
	}
}

void GameEngine::getMovementLines(Entity *entity, LineInfo *lineA,
								  LineInfo *lineB) {
	// Find points that will determine Lines
	lineA->startX = entity->getPosition()[0];
	lineA->startZ = entity->getPosition()[2];
	lineB->startX = entity->getPosition()[0];
	lineB->startZ = entity->getPosition()[2];
	// Avoid division by 0 if movement is only along 'z' axis
	if (entity->getTargetMovement()[0] == 0.0f) {
		lineA->startX += entity->getCollider()->width;
		lineB->startX -= entity->getCollider()->width;
		lineA->isVertical = true;
		lineA->q = lineA->startX;
		lineB->isVertical = true;
		lineB->q = lineB->startX;
	} else {
		// Find 'm' of direction from center to target center
		float m = entity->getTargetMovement()[2] /
				  entity->getTargetMovement()[0];  // TargetMovement is already
												   // a delta of coords
		lineA->m = m;
		lineB->m = m;
		lineA->isVertical = false;
		lineB->isVertical = false;

		// Find points based on shape
		if (entity->getCollider()->shape == Collider::Rectangle) {
			// Rectangle
			// TODO: for now we increase Z when we want to go down, but will
			// change soon
			if ((entity->getTargetMovement()[0] > 0.0f &&
				 entity->getTargetMovement()[2] > 0.0f) ||
				(entity->getTargetMovement()[0] < 0.0f &&
				 entity->getTargetMovement()[2] < 0.0f)) {
				// We are going Down-Right or Up-Left, take TopRight(A) and
				// BottomLeft(B) corners
				lineA->startX += entity->getCollider()->width;
				lineA->startZ -= entity->getCollider()
									 ->height;  // TODO: Change minus to plus
				lineB->startX -= entity->getCollider()->width;
				lineB->startZ += entity->getCollider()
									 ->height;  // TODO: Change plus to minus
			} else {
				// We are going Down-Left or Up-Right, take TopLeft(A) and
				// BottomRight(B) corners
				lineA->startX -= entity->getCollider()->width;
				lineA->startZ -= entity->getCollider()
									 ->height;  // TODO: Change minus to plus
				lineB->startX += entity->getCollider()->width;
				lineB->startZ += entity->getCollider()
									 ->height;  // TODO: Change plus to minus
			}
		} else {
			// Circle
			// mPrime = - 1 / m
			float mPerpendicular = -(1.0f / m);
			// q of line that will intercept startA and startB
			float q = entity->getPosition()[2] -
					  mPerpendicular * entity->getPosition()[0];

			float xOffset = mPerpendicular * entity->getCollider()->width;
			lineA->startX += xOffset;
			lineB->startX -= xOffset;
			// since we have 'mPerpendicular', 'q' and 'x' we can easily get 'z'
			lineA->startZ = mPerpendicular * lineA->startX + q;
			lineB->startZ = mPerpendicular * lineB->startX + q;
		}

		// With m and boh starting points, we can easily get 'q' for lineA and
		// lineB
		lineA->q = lineA->startZ - m * lineA->startX;
		lineB->q = lineB->startZ - m * lineB->startX;
	}

	// Add end point infos
	lineA->endX = lineA->startX + entity->getTargetMovement()[0];
	lineA->endZ = lineA->startZ + entity->getTargetMovement()[2];
	lineB->endX = lineB->startX + entity->getTargetMovement()[0];
	lineB->endZ = lineB->startZ + entity->getTargetMovement()[2];
}

bool GameEngine::hasCollisionCourse(LineInfo &lineA, LineInfo &lineB,
									Entity *entityB) {
	// Check to avoid errors
	if (lineA.isVertical || lineB.isVertical) {
		// Easy case, we can use doCollide() since we have a "standard"
		// rectangle
		Collider testCollider(Collider::Rectangle,
							  abs(lineA.startX - lineB.startX) / 2,
							  abs(lineA.startZ - lineA.endZ) / 2);
		std::vector<float> centerPos(3, 0);
		centerPos[0] = (lineA.startX + lineB.startX) / 2;
		centerPos[2] = (lineA.startZ + lineA.endZ) / 2;
		// return false;
		return doCollide(&testCollider, centerPos, entityB);
	}

	// Classic case
	const Collider *colliderB = entityB->getCollider();
	if (colliderB->shape == Collider::Rectangle) {
		// Rectangle
		// TODO:: change sign for Z when up is positive
		float leftX = entityB->getPosition()[0] - colliderB->width;
		float rightX = entityB->getPosition()[0] + colliderB->width;
		float upZ = entityB->getPosition()[2] - colliderB->height;
		float downZ = entityB->getPosition()[2] + colliderB->height;
		LineInfo tmpLine(leftX, upZ, rightX, upZ);
		if (isLineLineCollision(lineA, tmpLine) ||
			isLineLineCollision(lineB, tmpLine)) {
			return true;
		}
		tmpLine = LineInfo(leftX, downZ, rightX, downZ);
		if (isLineLineCollision(lineA, tmpLine) ||
			isLineLineCollision(lineB, tmpLine)) {
			return true;
		}
		tmpLine = LineInfo(leftX, upZ, leftX, downZ);
		if (isLineLineCollision(lineA, tmpLine) ||
			isLineLineCollision(lineB, tmpLine)) {
			return true;
		}
		tmpLine = LineInfo(rightX, upZ, rightX, downZ);
		if (isLineLineCollision(lineA, tmpLine) ||
			isLineLineCollision(lineB, tmpLine)) {
			return true;
		}
	} else {
		// Circle
		// To be true it must be one of the following
		// 1) Circle intersects with (at least one) line and point of
		// intersection is between start end end
		float xSquareCoeff = 1.0f;
		float xCoeff = -(2.0f * entityB->getPosition()[0]);
		float zSquareCoeff = 1.0f;
		float zCoeff = -(2.0f * entityB->getPosition()[2]);
		float cCoeff = colliderB->width + pow(entityB->getPosition()[0], 2) +
					   pow(entityB->getPosition()[2], 2);
		if (isLineCircleCollision(lineA, xSquareCoeff, xCoeff, zSquareCoeff,
								  zCoeff, cCoeff)) {
			return true;
		}
		if (isLineCircleCollision(lineB, xSquareCoeff, xCoeff, zSquareCoeff,
								  zCoeff, cCoeff)) {
			return true;
		}

		// 2) Circle is between two lines but with a tiny radius
		// TODO
	}
	return false;
}

bool GameEngine::isLineLineCollision(LineInfo &lineA, LineInfo &lineB) {
	// Easy: If both line are vertical
	if (lineA.isVertical && lineB.isVertical) {
		return lineA.startX == lineB.startX;
	}

	// Easy: If lines are parallel
	if (lineA.m == lineB.m) {
		return lineA.q == lineB.q;
	}

	float intersectionPointX = (lineB.q - lineA.q) / (lineA.m - lineB.m);
	// Only truly collide if intersec point is between start/end
	if ((lineA.startX <= intersectionPointX &&
		 intersectionPointX <= lineA.endX) ||
		(lineA.endX <= intersectionPointX &&
		 intersectionPointX <= lineA.startX)) {
		// Do same thing for other line
		if ((lineB.startX <= intersectionPointX &&
			 intersectionPointX <= lineB.endX) ||
			(lineB.endX <= intersectionPointX &&
			 intersectionPointX <= lineB.startX)) {
			return true;
		}
	}

	return false;
}

bool GameEngine::isLineCircleCollision(LineInfo &lineA, float &xSquareCoeff,
									   float &xCoeff, float &zSquareCoeff,
									   float &zCoeff, float &cCoeff) {
	xSquareCoeff += zSquareCoeff * pow(lineA.m, 2);
	xCoeff += zSquareCoeff * (2 * lineA.m * lineA.q);
	cCoeff += zSquareCoeff * pow(lineA.q, 2);
	xCoeff += zCoeff * lineA.m;
	cCoeff += zCoeff * lineA.q;

	float delta = pow(xCoeff, 2) - 4 * xSquareCoeff * cCoeff;
	if (delta < 0) return false;
	delta = sqrt(delta);
	float intersectionPointX = (-xCoeff + delta) / (2 * xSquareCoeff);
	// Point really collides only if is inside line's start/end point
	if ((lineA.startX <= intersectionPointX &&
		 intersectionPointX <= lineA.endX) ||
		(lineA.endX <= intersectionPointX &&
		 intersectionPointX <= lineA.startX)) {
		return true;
	}
	if (delta > 0) {
		intersectionPointX = (-xCoeff - delta) / (2 * xSquareCoeff);
		if ((lineA.startX <= intersectionPointX &&
			 intersectionPointX <= lineA.endX) ||
			(lineA.endX <= intersectionPointX &&
			 intersectionPointX <= lineA.startX)) {
			return true;
		}
	}
	return false;
}

bool GameEngine::doCollide(const Collider *colliderA,
						   const std::vector<float> &posA,
						   Entity *entityB) const {
	const Collider *colliderB = entityB->getCollider();
	if (!colliderA || !colliderB) return false;

	if (colliderA->shape == colliderB->shape) {
		float aXCenter = posA[0];
		float aYCenter = posA[2];
		float bXCenter = entityB->getPosition()[0];
		float bYCenter = entityB->getPosition()[2];
		// Circle with circle
		if (colliderA->shape == Collider::Circle) {
			float distance =
				sqrt(pow(aXCenter - bXCenter, 2) + pow(aYCenter - bYCenter, 2));
			return (distance <= colliderA->width + colliderB->width);
		}
		// Rectangle with rectangle
		else if (colliderA->shape == Collider::Rectangle) {
			return (abs(aXCenter - bXCenter) <=
						colliderA->width + colliderB->width &&
					abs(aYCenter - bYCenter) <=
						colliderA->height + colliderB->height);
		}
	}
	// Circle with rectangle
	else if (colliderA->shape == Collider::Circle &&
			 colliderB->shape == Collider::Rectangle) {
		return collisionCircleRectangle(colliderA, posA, entityB->getCollider(),
										entityB->getPosition());
	}
	// Rectangle with circle
	else if (colliderB->shape == Collider::Circle &&
			 colliderA->shape == Collider::Rectangle) {
		return collisionCircleRectangle(
			entityB->getCollider(), entityB->getPosition(), colliderA, posA);
	}
	return false;
}

bool GameEngine::collisionCircleRectangle(
	const Collider *circleCollider, const std::vector<float> &circlePos,
	const Collider *rectangleCollider,
	const std::vector<float> &rectanglePos) const {
	float closestX = circlePos[0];
	float closestY = circlePos[2];
	// Find closest X of rectangle shape to circle center
	if (closestX > rectanglePos[0] + rectangleCollider->width)
		closestX = rectanglePos[0] + rectangleCollider->width;
	else if (closestX < rectanglePos[0] - rectangleCollider->width)
		closestX = rectanglePos[0] - rectangleCollider->width;
	// Find closest Y of rectangle shape to circle center
	if (closestY > rectanglePos[2] + rectangleCollider->height)
		closestY = rectanglePos[2] + rectangleCollider->height;
	else if (closestY < rectanglePos[2] - rectangleCollider->height)
		closestY = rectanglePos[2] - rectangleCollider->height;
	// Is distance of closer point smaller than circle radius ?
	return sqrt(pow(circlePos[0] - closestX, 2) +
				pow(circlePos[2] - closestY, 2)) <= circleCollider->width;
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

		// TODO: set position of entities back to prev frame when they
		// collide
		moveEntities();

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
