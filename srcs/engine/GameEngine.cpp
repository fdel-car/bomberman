#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"

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

GameEngine::GameEngine(AGame *game) : _game(game) {
	_running = false;

	// Create interface class
	_gameRenderer = new GameRenderer(this, game);
	// Create audio manager
	_audioManager = new AudioManager();

	// Force load of first scene
	_sceneIdx = 0;
}

GameEngine::~GameEngine(void) {
	delete _audioManager;
	delete _gameRenderer;
}

float GameEngine::getDeltaTime(void) { return _deltaTime; }

GameRenderer const *GameEngine::getGameRenderer(void) const {
	return _gameRenderer;
}

void GameEngine::run(void) {
	// Init vars
	_running = true;
	_lastFrameTs = Clock::now();

	if (!initScene(_sceneIdx)) throw std::runtime_error("Cannot load scene!");
	int newSceneIdx = -1;
	// Start game loop
	while (_running) {
		// Get delta time in order to synch entities positions
		_frameTs = Clock::now();
		_deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(
						  _frameTs - _lastFrameTs)
						  .count()) /
					 1000.0f;
		_lastFrameTs = _frameTs;

		// Update inputs
		_gameRenderer->getUserInput();

		// TODO: Update game engine statuses (ex. when to quit)
		if (_running) {
			_running = !keyboardMap["ESCAPE"];
			if (!_running) break;
		}

		// Update game camera
		_camera->update();
		newSceneIdx = _camera->getNewSceneIdx();
		if (newSceneIdx != -1) break;
		newSceneIdx = _game->getSceneIndexByName(_camera->getNewSceneName());
		if (newSceneIdx != -1) break;

		// Update game entities states
		for (auto entity : _allEntities) {
			entity->update();
		}
		moveEntities();
		_gameRenderer->refreshWindow(_allEntities, _camera);
	}
	if (newSceneIdx != -1) {
		_sceneIdx = newSceneIdx;
		run();
	}
}

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

bool GameEngine::initScene(size_t newSceneIdx) {
	if (!_game) return false;
	_sceneIdx = newSceneIdx;
	if (!_game->loadSceneByIndex(_sceneIdx)) return false;

	_clearTmpEntities();
	_camera = _game->getCamera();
	_camera->initEntity(this);
	_camera->configGUI(_gameRenderer->getGUI());

	for (auto entity : _game->getEntities()) {
		_allEntities.push_back(entity);
		_allEntities.back()->initEntity(this);
	}
	return true;
}

// /!\ Function not tested yet
void GameEngine::_clearTmpEntities(void) {
	size_t idx = 0;
	for (auto entity : _allEntities) {
		if (entity->getTmpState())
			_allEntities.erase(_allEntities.begin() + idx);
		else
			idx++;
	}
}

void GameEngine::moveEntities(void) {
	const Collider *collider;
	bool collisionDetected = false;
	std::vector<Entity *> collidedEntities = std::vector<Entity *>();
	std::vector<Entity *> collidedTriggers = std::vector<Entity *>();
	glm::vec3 futureMovement = glm::vec3();
	glm::vec3 tmpFutureMovement = glm::vec3();
	LineInfo lineA;
	LineInfo lineB;

	size_t idx = 0;
	size_t idxOfCollision = 0;
	for (auto entity : _allEntities) {
		if (entity->getTargetMovement().x != 0.0f ||
			entity->getTargetMovement().z != 0.0f) {
			collider = entity->getCollider();
			collisionDetected = false;

			if (collider) {
				// Init vars before first loop
				collidedEntities = _allEntities;
				collidedEntities.erase(collidedEntities.begin() +
									   idx);  // Erase self entity from list
											  // that needs to be checked
				// To make Obj slide over obstacles we have to adapt
				// targetMovement to suitable pos
				futureMovement.x = entity->getTargetMovement().x;
				futureMovement.z = entity->getTargetMovement().z;
				bool firstLoop = true;
				bool isShortcut = false;
				while (collidedEntities.size() != 0) {
					idxOfCollision =
						checkCollision(entity, futureMovement, collidedEntities,
									   collidedTriggers);

					// Update entities to check collisions with
					if (idxOfCollision != 0) {
						collidedEntities.erase(
							collidedEntities.begin(),
							collidedEntities.begin() + idxOfCollision);
					}

					// If some collisions are detected, try with a different
					// movement
					if (collidedEntities.size() != 0) {
						// Safe break to avoid infinite loop
						float absX = abs(futureMovement.x);
						float absZ = abs(futureMovement.z);

						// Special test that need to be done only on first loop
						if (firstLoop) {
							// Try moving indipendently
							// one of the two axis (Only if its a two-axis move)
							if (futureMovement.x != 0.0f &&
								futureMovement.z != 0.0f) {
								tmpFutureMovement.x = futureMovement.x;
								tmpFutureMovement.z = 0.0f;
								idxOfCollision = checkCollision(
									entity, tmpFutureMovement, collidedEntities,
									collidedTriggers);
								if (idxOfCollision == collidedEntities.size()) {
									isShortcut = true;
									break;
								}
								tmpFutureMovement.x = 0.0f;
								tmpFutureMovement.z = futureMovement.z;
								idxOfCollision = checkCollision(
									entity, tmpFutureMovement, collidedEntities,
									collidedTriggers);
								if (idxOfCollision == collidedEntities.size()) {
									isShortcut = true;
									break;
								}
							}

							// Special cases for Circle, will be able to
							// "circle around" obstacles more easily if more
							// than half is outside collision
							if (collider->shape == Collider::Circle) {
								// Only try to move along the most important
								// targetMovement
								if (absX >= absZ) {
									// Try slide under obstacle
									float colliderZ =
										collidedEntities[0]->getPosition().z +
										collidedEntities[0]
											->getCollider()
											->height;
									if (entity->getPosition().z > colliderZ) {
										tmpFutureMovement.z =
											colliderZ -
											(entity->getPosition().z -
											 entity->getCollider()->height);
										if (tmpFutureMovement.z > (absX / 2)) {
											tmpFutureMovement.z = absX / 2;
										}
										if (tmpFutureMovement.z < 0.0f)
											tmpFutureMovement.z = EPSILON;
										else if (tmpFutureMovement.z < EPSILON)
											tmpFutureMovement.z += EPSILON;
										// Move along X will be less than Z so
										// that no collision will occur
										tmpFutureMovement.x =
											tmpFutureMovement.z * 0.5f;
										if (futureMovement.x < 0)
											tmpFutureMovement.x *= -1.0f;
										idxOfCollision = checkCollision(
											entity, tmpFutureMovement,
											collidedEntities, collidedTriggers);
										if (idxOfCollision ==
											collidedEntities.size()) {
											isShortcut = true;
											break;
										}
									}
									// Try slide over obstacle
									colliderZ -= 2.0f * collidedEntities[0]
															->getCollider()
															->height;
									if (entity->getPosition().z < colliderZ) {
										tmpFutureMovement.z =
											(entity->getPosition().z +
											 entity->getCollider()->height) -
											colliderZ;
										if (tmpFutureMovement.z > (absX / 2)) {
											tmpFutureMovement.z = absX / 2;
										}
										if (tmpFutureMovement.z < 0.0f)
											tmpFutureMovement.z = EPSILON;
										else if (tmpFutureMovement.z < EPSILON)
											tmpFutureMovement.z += EPSILON;
										// Move along X will be less than Z so
										// that no collision will occur
										tmpFutureMovement.x =
											tmpFutureMovement.z * 0.5f;
										if (futureMovement.x < 0)
											tmpFutureMovement.x *= -1.0f;
										tmpFutureMovement.z *= -1;
										idxOfCollision = checkCollision(
											entity, tmpFutureMovement,
											collidedEntities, collidedTriggers);
										if (idxOfCollision ==
											collidedEntities.size()) {
											isShortcut = true;
											break;
										}
									}
								}
								if (absZ >= absX) {
									// Try slide right of obstacle
									float colliderX =
										collidedEntities[0]->getPosition().x +
										collidedEntities[0]
											->getCollider()
											->width;
									if (entity->getPosition().x > colliderX) {
										tmpFutureMovement.x =
											colliderX -
											(entity->getPosition().x -
											 entity->getCollider()->width);
										if (tmpFutureMovement.x > (absZ / 2)) {
											tmpFutureMovement.x = absZ / 2;
										}
										if (tmpFutureMovement.x < 0.0f)
											tmpFutureMovement.x = EPSILON;
										else if (tmpFutureMovement.x < EPSILON)
											tmpFutureMovement.x += EPSILON;
										// Move along Z will be less than X so
										// that no collision will occur
										tmpFutureMovement.z =
											tmpFutureMovement.x * 0.5f;
										if (futureMovement.z < 0)
											tmpFutureMovement.z *= -1.0f;
										idxOfCollision = checkCollision(
											entity, tmpFutureMovement,
											collidedEntities, collidedTriggers);
										if (idxOfCollision ==
											collidedEntities.size()) {
											isShortcut = true;
											break;
										}
									}
									// Try slide over obstacle
									colliderX -= 2.0f * collidedEntities[0]
															->getCollider()
															->width;
									if (entity->getPosition().x < colliderX) {
										tmpFutureMovement.x =
											(entity->getPosition().x +
											 entity->getCollider()->width) -
											colliderX;
										if (tmpFutureMovement.x > (absZ / 2)) {
											tmpFutureMovement.x = absZ / 2;
										}
										if (tmpFutureMovement.x < 0.0f)
											tmpFutureMovement.x = EPSILON;
										else if (tmpFutureMovement.x < EPSILON)
											tmpFutureMovement.x += EPSILON;
										// Move along X will be less than Z so
										// that no collision will occur
										tmpFutureMovement.z =
											tmpFutureMovement.x * 0.5f;
										if (futureMovement.z < 0)
											tmpFutureMovement.z *= -1.0f;
										tmpFutureMovement.x *= -1;
										idxOfCollision = checkCollision(
											entity, tmpFutureMovement,
											collidedEntities, collidedTriggers);
										if (idxOfCollision ==
											collidedEntities.size()) {
											isShortcut = true;
											break;
										}
									}
								}
							}

							firstLoop = false;
						}

						// Slightly decrease futureMovement to see if
						// smaller movement can be performed
						futureMovement.x /= 2.0f;
						futureMovement.z /= 2.0f;

						if (absX <= EPSILON && absZ <= EPSILON) {
							collidedTriggers.clear();
							break;
						}
					}
				}

				if (isShortcut) {
					futureMovement.x = tmpFutureMovement.x;
					futureMovement.z = tmpFutureMovement.z;
					collidedEntities.clear();
				}

				// TODO: trigger all triggers
				// for (auto triggerEntity : collidedTriggers) {
				// }
				if (collidedEntities.size() == 0) {
					entity->translate(futureMovement);
				}
			} else {
				// Skip checks if entity doesnt have a collider
				entity->translate(entity->getTargetMovement());
			}
		}
		idx++;
	}
}

size_t GameEngine::checkCollision(Entity *entity, glm::vec3 &futureMovement,
								  std::vector<Entity *> &collidedEntities,
								  std::vector<Entity *> &collidedTriggers) {
	if (collidedEntities.size() == 0) return 0;
	// Init
	glm::vec3 futurePos = glm::vec3();
	LineInfo lineA;
	LineInfo lineB;
	collidedTriggers.clear();

	// 1) Find lines that inglobe movement
	getMovementLines(entity, futureMovement, &lineA, &lineB);
	// If lines are vertical, create Rectangle Collider only
	// once
	Collider moveCollider(Collider::Rectangle, 0.0f, 0.0f);
	glm::vec3 centerPos = glm::vec3();
	if (lineA.isVertical) {
		moveCollider.width = abs(lineA.startX - lineB.startX) / 2.0f;
		moveCollider.height = abs(lineA.startZ - lineA.endZ) / 2.0f;
		centerPos.x = (lineA.startX + lineB.startX) / 2;
		centerPos.z = (lineA.startZ + lineA.endZ) / 2;
	}

	// Simulate movement
	futurePos.x = entity->getPosition().x + futureMovement.x;
	futurePos.z = entity->getPosition().z + futureMovement.z;

	size_t idxToTest = 0;
	for (auto entityToTest : collidedEntities) {
		// TODO: skip collision with object that cannot collide with ?
		if (entityToTest->getCollider() != nullptr) {
			// 2) Check if final position collides with smth
			// 3) Check if any other obj collides with lineA/lineB
			if (doCollide(entity->getCollider(), futurePos, entityToTest) ||
				(lineA.isVertical &&
				 doCollide(&moveCollider, centerPos, entityToTest)) ||
				(!lineA.isVertical &&
				 hasCollisionCourse(lineA, lineB, entityToTest))) {
				// TODO: handle if is physical collision or trigger
				break;
			}
		}
		idxToTest++;
	}

	return idxToTest;
}

void GameEngine::getMovementLines(Entity *entity, glm::vec3 &targetMovement,
								  LineInfo *lineA, LineInfo *lineB) {
	// Find points that will determine Lines
	lineA->startX = entity->getPosition().x;
	lineA->startZ = entity->getPosition().z;
	lineB->startX = entity->getPosition().x;
	lineB->startZ = entity->getPosition().z;
	// Avoid division by 0 if movement is only along 'z' axis
	if (targetMovement.x == 0.0f) {
		lineA->startX += entity->getCollider()->width;
		lineB->startX -= entity->getCollider()->width;
		lineA->isVertical = true;
		lineA->q = lineA->startX;
		lineB->isVertical = true;
		lineB->q = lineB->startX;
	} else {
		// Find 'm' of direction from center to target center
		float m =
			targetMovement.z / targetMovement.x;  // TargetMovement is already
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
			if ((targetMovement.x > 0.0f && targetMovement.z > 0.0f) ||
				(targetMovement.x < 0.0f && targetMovement.z < 0.0f)) {
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
			float q = entity->getPosition().z -
					  mPerpendicular * entity->getPosition().x;

			float xOffset = mPerpendicular * entity->getCollider()->width;
			lineA->startX += xOffset;
			lineB->startX -= xOffset;
			// since we have 'mPerpendicular', 'q' and 'x' we can easily get
			// 'z'
			lineA->startZ = mPerpendicular * lineA->startX + q;
			lineB->startZ = mPerpendicular * lineB->startX + q;
		}

		// With m and boh starting points, we can easily get 'q' for lineA
		// and lineB
		lineA->q = lineA->startZ - m * lineA->startX;
		lineB->q = lineB->startZ - m * lineB->startX;
	}

	// Add end point infos
	lineA->endX = lineA->startX + targetMovement.x;
	lineA->endZ = lineA->startZ + targetMovement.z;
	lineB->endX = lineB->startX + targetMovement.x;
	lineB->endZ = lineB->startZ + targetMovement.z;
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
		glm::vec3 centerPos = glm::vec3();
		centerPos.x = (lineA.startX + lineB.startX) / 2;
		centerPos.z = (lineA.startZ + lineA.endZ) / 2;
		// return false;
		return doCollide(&testCollider, centerPos, entityB);
	}

	// Classic case
	const Collider *colliderB = entityB->getCollider();
	if (colliderB->shape == Collider::Rectangle) {
		// Rectangle
		// TODO:: change sign for Z when up is positive
		float leftX = entityB->getPosition().x - colliderB->width;
		float rightX = entityB->getPosition().x + colliderB->width;
		float upZ = entityB->getPosition().z - colliderB->height;
		float downZ = entityB->getPosition().z + colliderB->height;
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
		float xCoeff = -(2.0f * entityB->getPosition().x);
		float zSquareCoeff = 1.0f;
		float zCoeff = -(2.0f * entityB->getPosition().z);
		float cCoeff = colliderB->width + pow(entityB->getPosition().x, 2) +
					   pow(entityB->getPosition().z, 2);
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
bool GameEngine::doCollide(const Collider *colliderA, const glm::vec3 &posA,
						   Entity *entityB) const {
	const Collider *colliderB = entityB->getCollider();
	if (!colliderA || !colliderB) return false;
	if (colliderA->shape == colliderB->shape) {
		float aXCenter = posA.x;
		float aZCenter = posA.z;
		float bXCenter = entityB->getPosition().x;
		float bZCenter = entityB->getPosition().z;
		// Circle with circle
		if (colliderA->shape == Collider::Circle) {
			float distance =
				sqrt(pow(aXCenter - bXCenter, 2) + pow(aZCenter - bZCenter, 2));
			distance -= colliderA->width +
						colliderB->width;  // If value is greater than 0
										   // then there is no collision
			return (distance <= EPSILON);
		}
		// Rectangle with rectangle
		else if (colliderA->shape == Collider::Rectangle) {
			float distance = abs(aXCenter - bXCenter);
			distance -= colliderA->width +
						colliderB->width;  // If value is greater than 0
										   // then there is no collision
			if (distance <= EPSILON) {
				// Do same for Z
				distance = abs(aZCenter - bZCenter);
				distance -= colliderA->height + colliderB->height;
				return (distance <= EPSILON);
			}
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

bool GameEngine::collisionCircleRectangle(const Collider *circleCollider,
										  const glm::vec3 &circlePos,
										  const Collider *rectangleCollider,
										  const glm::vec3 &rectanglePos) const {
	float closestX = circlePos.x;
	float closestZ = circlePos.z;
	// Find closest X of rectangle shape to circle center
	if (closestX > rectanglePos.x + rectangleCollider->width)
		closestX = rectanglePos.x + rectangleCollider->width;
	else if (closestX < rectanglePos.x - rectangleCollider->width)
		closestX = rectanglePos.x - rectangleCollider->width;
	// Find closest Z of rectangle shape to circle center
	if (closestZ > rectanglePos.z + rectangleCollider->height)
		closestZ = rectanglePos.z + rectangleCollider->height;
	else if (closestZ < rectanglePos.z - rectangleCollider->height)
		closestZ = rectanglePos.z - rectangleCollider->height;
	// Is distance of closer point smaller than circle radius ?
	return ((sqrt(pow(circlePos.x - closestX, 2) +
				  pow(circlePos.z - closestZ, 2))) -
				circleCollider->width <=
			EPSILON);
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
