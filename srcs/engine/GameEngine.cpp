#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"

// std::thread *_loadSceneThread = nullptr;

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

GameEngine::RectanglePoints::RectanglePoints(Entity *entity,
											 glm::vec3 movement) {
	top = entity->getPosition().z - entity->getCollider()->height;
	bot = entity->getPosition().z + entity->getCollider()->height;
	left = entity->getPosition().x - entity->getCollider()->width;
	right = entity->getPosition().x + entity->getCollider()->width;

	if (movement.x > 0.0f)
		right += movement.x;
	else if (movement.x < 0.0f)
		left += movement.x;
	if (movement.z > 0.0f)
		bot += movement.z;
	else if (movement.z < 0.0f)
		top += movement.z;

	// Make Rectangle a little bigger
	top += -EPSILON;
	bot += EPSILON;
	left += -EPSILON;
	right += EPSILON;
}

GameEngine::GameEngine(AGame *game)
	: _game(game),
	  _allEntities(std::vector<Entity *>()),
	  _newEntities(std::vector<Entity *>()),
	  _initialCollisionMap(std::map<size_t, std::vector<size_t>>()),
	  _collisionTable(game->getCollisionTable()) {
	// Create interface class
	_gameRenderer = new GameRenderer(this, _game);
	_game->setGameRenderer(_gameRenderer);
	// Create audio manager
	_audioManager = new AudioManager(_game->getStartingMusicVolume(),
									 _game->getStartingSoundsVolume());
	_game->setAudioManager(_audioManager);

	// Force load of first scene
	_sceneIdx = _game->getFirstSceneIdx();

	// Thread atomic Int
	_sceneState = BACKGROUND_LOAD_NEEDED;
	_checkLoadSceneIsGood = false;

	// Set LoadingScene variable
	_setLoadingSceneVariables();
}

GameEngine::~GameEngine(void) {
	if (_sceneState != BACKGROUND_LOAD_STARTED) {
		_unloadScene();
	}
	delete _audioManager;
	delete _gameRenderer;
}

float GameEngine::getDeltaTime(void) { return _deltaTime; }

GameRenderer const *GameEngine::getGameRenderer(void) const {
	return _gameRenderer;
}

void GameEngine::addNewEntity(Entity *entity) {
	_newEntities.push_back(entity);
	_newEntities.back()->initEntity(this);
}

void GameEngine::run(void) {
	if (_sceneState == BACKGROUND_LOAD_NEEDED) {
		_sceneState = BACKGROUND_LOAD_STARTED;
		_unloadScene();
		_camera = _loadingCamera;
		_light = _loadingLight;
		_skybox = _loadingSkybox;
		_allEntities = _loadingAllEntities;

		// Init thread to load the scene we want
		_checkLoadSceneIsGood = false;
		_loadSceneThread =
			new std::thread(&GameEngine::_loadScene, this, _sceneIdx,
							&_sceneState, &_checkLoadSceneIsGood);

		// Wait for other thread to finish
		while (_sceneState != BACKGROUND_LOAD_FINISHED) {
			_gameRenderer->getUserInput();
			_camera->update();
			for (auto entity : _allEntities) {
				entity->update();
			}
			_gameRenderer->refreshWindow(_allEntities, _camera, _light,
										 _skybox);
		}
		// Free thread
		_loadSceneThread->join();
		delete _loadSceneThread;
		_loadSceneThread = nullptr;

		// Load level
		if (_checkLoadSceneIsGood == false)
			throw std::runtime_error("Cannot load scene!");
		if (!_initScene(_sceneIdx))
			throw std::runtime_error("Cannot load scene " +
									 std::to_string(_sceneIdx) + "!");
		_sceneState = BACKGROUND_LOAD_NOT_NEEDED;
	}

	// Start game loop
	int newSceneIdx = -1;
	_lastFrameTs = Clock::now();
	while (true) {
		// Get delta time in order to synch entities positions
		_frameTs = Clock::now();
		_deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(
						  _frameTs - _lastFrameTs)
						  .count()) /
					 1000000.0f;
		_lastFrameTs = _frameTs;

		// Update inputs
		for (auto &key : _keyboardMap)
			key.second.prevFrame = key.second.currFrame;

		_gameRenderer->getUserInput();

		// Update game camera
		_camera->update();
		newSceneIdx = _camera->getNewSceneIdx();
		if (newSceneIdx != -1) break;
		newSceneIdx = _game->getSceneIndexByName(_camera->getNewSceneName());
		if (newSceneIdx != -1) break;

		if (!_camera->isGameRunning()) break;

		// Freeze everything else if camera tells so
		if (!_camera->isPause()) {
			if (_light) _light->update();
			// Update game entities states
			for (auto entity : _allEntities) {
				entity->update();
			}

			// Merge new game entities
			if (!_newEntities.empty()) {
				std::vector<Entity *> collidedEntities =
					std::vector<Entity *>();
				std::vector<Entity *> triggeredEntities =
					std::vector<Entity *>();  // we dont really use this vector,
											  // it's just needed by func
				std::vector<Entity *> entitiesToTest = _allEntities;
				entitiesToTest.insert(entitiesToTest.end(),
									  _newEntities.begin(), _newEntities.end());
				std::vector<size_t> initialCollisions = std::vector<size_t>();
				for (auto newEntity : _newEntities) {
					if (newEntity->getCollider() == nullptr ||
						newEntity->getCollider()->isTrigger)
						continue;
					collidedEntities.clear();
					initialCollisions.clear();
					_getPossibleCollisions(newEntity, collidedEntities,
										   triggeredEntities, entitiesToTest);
					// Possible collision that have been detected are sure to
					// collide since newEntities do not have any targetMovement
					// yet
					if (!collidedEntities.empty()) {
						for (auto collidedEntity : collidedEntities) {
							if (_doCollide(newEntity->getCollider(),
										   newEntity->getPosition(),
										   collidedEntity)) {
								initialCollisions.push_back(
									collidedEntity->getId());
								// Add for other entity too
								_initialCollisionMap[collidedEntity->getId()]
									.push_back(newEntity->getId());
							}
						}
						if (!initialCollisions.empty())
							_initialCollisionMap[newEntity->getId()] =
								initialCollisions;
					}
				}
				_allEntities.insert(_allEntities.end(), _newEntities.begin(),
									_newEntities.end());
				_newEntities.clear();
			}

			// Do movement (with collisions + trigger detection)
			_moveEntities();

			// Delete game entities if needed
			for (size_t idx = _allEntities.size() - 1;
				 idx < _allEntities.size(); idx--) {
				if (_allEntities[idx]->needsToBeDestroyed()) {
					// Erase possible ingame ref
					if (_initialCollisionMap.find(_allEntities[idx]->getId()) !=
						_initialCollisionMap.end())
						_initialCollisionMap.erase(_allEntities[idx]->getId());
					for (auto &initialCollisions : _initialCollisionMap) {
						for (size_t idIdx = 0;
							 idIdx < initialCollisions.second.size(); idIdx++) {
							if (initialCollisions.second[idIdx] ==
								_allEntities[idx]->getId())
								initialCollisions.second.erase(
									initialCollisions.second.begin() + idIdx);
						}
					}
					delete _allEntities[idx];
					_allEntities.erase(_allEntities.begin() + idx);
				}
			}

			// Check if there are changes for _initialCollisionMap
			if (!_initialCollisionMap.empty()) {
				std::vector<size_t> idxToDelete;
				Entity *entityA;
				// Entity *entityB;
				for (auto &initialCollisions : _initialCollisionMap) {
					entityA = getEntityById(initialCollisions.first);
					// Just to be sure
					if (entityA == nullptr) {
						idxToDelete.push_back(initialCollisions.first);
						continue;
					}
					for (size_t idIdx = initialCollisions.second.size() - 1;
						 idIdx < initialCollisions.second.size(); idIdx--) {
						if (!_doCollide(entityA->getCollider(),
										entityA->getPosition(),
										getEntityById(
											initialCollisions.second[idIdx]))) {
							initialCollisions.second.erase(
								initialCollisions.second.begin() + idIdx);
						}
					}
					if (initialCollisions.second.empty())
						idxToDelete.push_back(initialCollisions.first);
				}
				for (auto idx : idxToDelete) {
					_initialCollisionMap.erase(idx);
				}
			}
		}
		_gameRenderer->refreshWindow(_allEntities, _camera, _light, _skybox);

		if (_game->needResolutionChange) _setNewResolution();
	}
	if (newSceneIdx != -1) {
		_sceneIdx = newSceneIdx;
		_sceneState = BACKGROUND_LOAD_NEEDED;
		run();
	}
}

Entity *GameEngine::getEntityById(size_t id) {
	Entity *foundElem = nullptr;
	for (auto entity : _allEntities) {
		if (entity->getId() == id) {
			foundElem = entity;
			break;
		}
	}
	return foundElem;
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

void GameEngine::_setSceneVariables(void) {
	_camera = _game->getCamera();
	if (_camera == nullptr)
		throw std::runtime_error(
			"\033[0;31m:Error:\033[0m No camera were created in the loaded "
			"scene.");
	_camera->initEntity(this);
	_camera->configGUI(_gameRenderer->getGUI());

	_skybox = _game->getSkybox();
	if (_skybox != nullptr) {
		_skybox->_initBuffer();
		_skybox->_initCubeMap();
		_skybox->initEntity(this);
	}

	_light = _game->getLight();
	if (_light == nullptr) {
		std::cerr << "\033[0;33m:Warning:\033[0m There is no light in the "
					 "loaded scene, a default one was generated but you should "
					 "definitely add one"
				  << std::endl;
		_light = new Light(glm::vec2(-10.0, 10.0), glm::vec3(0.0f));
	}
	_light->initEntity(this);

	for (auto entity : _game->getEntities()) {
		_allEntities.push_back(entity);
		_allEntities.back()->initEntity(this);
	}
}

void GameEngine::_setLoadingSceneVariables(void) {
	_loadingCamera = _game->getLoadingCamera();
	if (_loadingCamera == nullptr)
		throw std::runtime_error(
			"\033[0;31m:Error:\033[0m No camera were created in the loaded "
			"scene.");
	_loadingCamera->initEntity(this);
	_loadingCamera->configGUI(_gameRenderer->getGUI());

	_loadingSkybox = _game->getLoadingSkybox();
	if (_loadingSkybox != nullptr) {
		_loadingSkybox->_initBuffer();
		_loadingSkybox->_initCubeMap();
		_loadingSkybox->initEntity(this);
	}

	_loadingLight = _game->getLoadingLight();
	if (_loadingLight == nullptr)
		std::cerr << "\033[0;33m:Warning:\033[0m There is no light in the "
					 "loaded scene, you should definitely add one"
				  << std::endl;
	else
		_loadingLight->initEntity(this);

	for (auto entity : _game->getLoadingEntities()) {
		_loadingAllEntities.push_back(entity);
		_loadingAllEntities.back()->initEntity(this);
	}
}

bool GameEngine::_initScene(size_t newSceneIdx) {
	if (!_game) return false;
	// Add new entities
	_sceneIdx = newSceneIdx;
	_gameRenderer->initModelsMeshes();
	_setSceneVariables();
	return true;
}

void GameEngine::_unloadScene(void) {
	// Clear not added entities
	if (_newEntities.empty() != true) {
		for (size_t idx = 0; idx < _newEntities.size(); idx++) {
			delete _newEntities[idx];
		}
		_newEntities.clear();
	}
	// Clear prev entities
	if (_allEntities.empty() != true) {
		for (size_t idx = _allEntities.size() - 1; idx < _allEntities.size();
			 idx--) {
			// TODO: add logic for entities that survive between scenes
			delete _allEntities[idx];
			_allEntities.erase(_allEntities.begin() + idx);
		}
	}
	if (_light != nullptr) {
		delete _light;
		_light = nullptr;
	}
	if (_camera != nullptr) {
		delete _camera;
		_camera = nullptr;
	}
	if (_skybox != nullptr) {
		delete _skybox;
		_skybox = nullptr;
	}
	_initialCollisionMap.clear();
}

void GameEngine::_loadScene(size_t newSceneIdx, std::atomic_int *_sceneState,
							bool *_checkLoadSceneIsGood) {
	_game->loadSceneByIndex(newSceneIdx, _sceneState, _checkLoadSceneIsGood);
}

void GameEngine::_moveEntities(void) {
	const Collider *collider;
	bool isShortcut = false;
	std::vector<Entity *> collidedEntitiesBck = std::vector<Entity *>();
	std::vector<Entity *> collidedEntities = std::vector<Entity *>();
	std::vector<Entity *> collidedTriggers = std::vector<Entity *>();
	Entity *shortcutEntity;
	glm::vec3 futureMovement = glm::vec3();
	glm::vec3 shortcutMovement = glm::vec3();
	LineInfo lineA;
	LineInfo lineB;
	size_t idx = 0;

	size_t idxOfCollision = 0;
	bool firstLoop = true;
	bool hasCollided = true;
	for (auto entity : _allEntities) {
		if (!entity->needsToBeDestroyed()) {
			collider = entity->getCollider();
			collidedEntitiesBck.clear();
			collidedEntities.clear();
			collidedTriggers.clear();
			isShortcut = false;
			firstLoop = true;
			hasCollided = false;
			shortcutEntity = nullptr;
			futureMovement = entity->getTargetMovement();

			if (futureMovement.x == 0.0f && futureMovement.z == 0.0f &&
				(collider == nullptr || !collider->isTrigger))
				continue;

			// Skip checks if entity doesnt have a collider
			if (collider != nullptr) {
				// Init vars before first loop
				_getPossibleCollisions(entity, collidedEntities,
									   collidedTriggers, _allEntities);
				collidedEntitiesBck = collidedEntities;
				futureMovement.x = entity->getTargetMovement().x;
				futureMovement.z = entity->getTargetMovement().z;

				// Apply shorter and shorter movement until no collision is
				// detected
				while (collidedEntities.size() != 0) {
					idxOfCollision = _checkCollision(entity, futureMovement,
													 collidedEntities);
					// Update entities to check collisions with
					if (idxOfCollision != 0) {
						collidedEntities.erase(
							collidedEntities.begin(),
							collidedEntities.begin() + idxOfCollision);
					}

					// If some collisions are detected, try with a different
					// movement
					if (collidedEntities.size() != 0) {
						if (firstLoop) {
							// Special test that need to be done only on
							// first loop
							if (_tryShortcut(
									entity, futureMovement, shortcutMovement,
									collidedEntities[0], collidedEntitiesBck)) {
								shortcutEntity = collidedEntities[0];
								isShortcut = true;
								futureMovement = shortcutMovement;
								break;
							}
							firstLoop = false;
						}

						// Loop again only for big movements
						if (abs(futureMovement.x) <= 0.5f &&
							abs(futureMovement.z) <= 0.5f) {
							break;
						}
						// Slightly decrease futureMovement to see if
						// smaller movement can be performed
						futureMovement.x /= 2.0f;
						futureMovement.z /= 2.0f;
					}
				}

				// Collide with colliders (do not consider shortcut move yet)
				while (!collidedEntitiesBck.empty() &&
					   !entity->needsToBeDestroyed()) {
					idx = _checkCollision(entity, futureMovement,
										  collidedEntitiesBck);
					if (idx != collidedEntitiesBck.size()) {
						hasCollided = true;
						// Other entity will always be a collider
						if (!collidedEntitiesBck[idx]->needsToBeDestroyed())
							collidedEntitiesBck[idx]->onCollisionEnter(entity);
						// Check if we are a trigger or a collider
						if (!entity->needsToBeDestroyed()) {
							if (collider->isTrigger)
								entity->onTriggerEnter(
									collidedEntitiesBck[idx]);
							else
								entity->onCollisionEnter(
									collidedEntitiesBck[idx]);
						}
						// Clear entities
						collidedEntitiesBck.erase(
							collidedEntitiesBck.begin(),
							collidedEntitiesBck.begin() + idx + 1);
					} else {
						collidedEntitiesBck.clear();
					}
				}
				// Apply shortcut and clear collisions
				if (isShortcut) {
					hasCollided = false;
					futureMovement = shortcutMovement;
					collidedEntities.clear();

					// Trigger on shortcutted entity

					// Other entity will always be a collider
					if (!shortcutEntity->needsToBeDestroyed())
						shortcutEntity->onCollisionEnter(entity);
					// Check if we are a trigger or a collider
					if (!entity->needsToBeDestroyed()) {
						if (collider->isTrigger)
							entity->onTriggerEnter(shortcutEntity);
						else
							entity->onCollisionEnter(shortcutEntity);
					}
				}
				// Trigger all triggers
				while (!collidedTriggers.empty() &&
					   !entity->needsToBeDestroyed()) {
					idx = _checkCollision(entity, futureMovement,
										  collidedTriggers);
					if (idx != collidedTriggers.size()) {
						// Other entity will always be a trigger
						collidedTriggers[idx]->onTriggerEnter(entity);
						// Check if we are a trigger or a collider
						if (collider->isTrigger)
							entity->onTriggerEnter(collidedTriggers[idx]);
						else
							entity->onCollisionEnter(collidedTriggers[idx]);
						// Clear entities
						collidedTriggers.erase(
							collidedTriggers.begin(),
							collidedTriggers.begin() + idx + 1);
					} else
						collidedTriggers.clear();
				}
			}

			if ((futureMovement.x != 0 || futureMovement.z != 0) &&
				!hasCollided && !entity->needsToBeDestroyed()) {
				entity->translate(futureMovement);
			}
		}
	}
}
void GameEngine::_getPossibleCollisions(
	Entity *entity, std::vector<Entity *> &possibleCollisions,
	std::vector<Entity *> &possibleTriggers,
	std::vector<Entity *> &entitiesToTest) {
	const Collider *collider = entity->getCollider();

	if (collider) {
		// Skip collision with object that cannot collide with
		bool canCollide = false;
		RectanglePoints rectanglePoints(entity, entity->getTargetMovement());
		for (size_t i = 0; i < entitiesToTest.size(); i++) {
			canCollide = false;
			// Skip self
			if (entity->getId() == entitiesToTest[i]->getId()) continue;

			// Skip if dead entity
			if (entity->needsToBeDestroyed()) continue;

			// Skip if it's an initialCollision
			if (_initialCollisionMap.find(entitiesToTest[i]->getId()) !=
					_initialCollisionMap.end() &&
				std::find(
					_initialCollisionMap[entitiesToTest[i]->getId()].begin(),
					_initialCollisionMap[entitiesToTest[i]->getId()].end(),
					entity->getId()) !=
					_initialCollisionMap[entitiesToTest[i]->getId()].end())
				continue;

			// Compare Layers
			if (entitiesToTest[i]->getCollider() != nullptr &&
				_collisionTable[collider->layerTag]
							   [entitiesToTest[i]->getCollider()->layerTag]) {
				// Fast check to know if is remotely possible that a
				// collision may occurr
				RectanglePoints otherPoints(entitiesToTest[i]);
				if (rectanglePoints.top <= otherPoints.bot &&
					otherPoints.top <= rectanglePoints.bot) {
					if (rectanglePoints.left <= otherPoints.right &&
						otherPoints.left <= rectanglePoints.right)
						canCollide = true;
				}
			}

			if (canCollide) {
				(entitiesToTest[i]->getCollider()->isTrigger)
					? possibleTriggers.push_back(entitiesToTest[i])
					: possibleCollisions.push_back(entitiesToTest[i]);
			}
		}
	}
}

size_t GameEngine::_checkCollision(Entity *entity, glm::vec3 &futureMovement,
								   std::vector<Entity *> &collidedEntities) {
	if (collidedEntities.size() == 0) return 0;
	// Init
	glm::vec3 futurePos = glm::vec3();
	LineInfo lineA;
	LineInfo lineB;

	// 1) Find lines that inglobe movement
	_getMovementLines(entity, futureMovement, &lineA, &lineB);
	// If lines are vertical, create Rectangle Collider only
	// once
	int layer1 = entity->getCollider()->layerTag;
	Collider moveCollider(Collider::Rectangle, layer1, 0.0f, 0.0f);
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
		// 2) Check if final position collides with smth
		// 3) Check if any other obj collides with lineA/lineB
		if (!entityToTest->needsToBeDestroyed() &&
			(_doCollide(entity->getCollider(), futurePos, entityToTest) ||
			 (lineA.isVertical &&
			  _doCollide(&moveCollider, centerPos, entityToTest)) ||
			 (!lineA.isVertical &&
			  _hasCollisionCourse(lineA, lineB, layer1, entityToTest)))) {
			break;
		}
		idxToTest++;
	}

	return idxToTest;
}

void GameEngine::_getMovementLines(Entity *entity, glm::vec3 &targetMovement,
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

bool GameEngine::_hasCollisionCourse(LineInfo &lineA, LineInfo &lineB,
									 int layerTag, Entity *entityB) {
	// Check to avoid errors
	if (lineA.isVertical || lineB.isVertical) {
		// Easy case, we can use _doCollide() since we have a "standard"
		// rectangle
		Collider testCollider(Collider::Rectangle, layerTag,
							  abs(lineA.startX - lineB.startX) / 2,
							  abs(lineA.startZ - lineA.endZ) / 2);
		glm::vec3 centerPos = glm::vec3();
		centerPos.x = (lineA.startX + lineB.startX) / 2;
		centerPos.z = (lineA.startZ + lineA.endZ) / 2;
		// return false;
		return _doCollide(&testCollider, centerPos, entityB);
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
		if (_isLineLineCollision(lineA, tmpLine) ||
			_isLineLineCollision(lineB, tmpLine)) {
			return true;
		}
		tmpLine = LineInfo(leftX, downZ, rightX, downZ);
		if (_isLineLineCollision(lineA, tmpLine) ||
			_isLineLineCollision(lineB, tmpLine)) {
			return true;
		}
		tmpLine = LineInfo(leftX, upZ, leftX, downZ);
		if (_isLineLineCollision(lineA, tmpLine) ||
			_isLineLineCollision(lineB, tmpLine)) {
			return true;
		}
		tmpLine = LineInfo(rightX, upZ, rightX, downZ);
		if (_isLineLineCollision(lineA, tmpLine) ||
			_isLineLineCollision(lineB, tmpLine)) {
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
		if (_isLineCircleCollision(lineA, xSquareCoeff, xCoeff, zSquareCoeff,
								   zCoeff, cCoeff)) {
			return true;
		}
		if (_isLineCircleCollision(lineB, xSquareCoeff, xCoeff, zSquareCoeff,
								   zCoeff, cCoeff)) {
			return true;
		}

		// 2) Circle is between two lines but with a tiny radius
		// TODO
	}
	return false;
}

bool GameEngine::_isLineLineCollision(LineInfo &lineA, LineInfo &lineB) {
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

bool GameEngine::_isLineCircleCollision(LineInfo &lineA, float &xSquareCoeff,
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
bool GameEngine::_doCollide(const Collider *colliderA, const glm::vec3 &posA,
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
		return _collisionCircleRectangle(
			colliderA, posA, entityB->getCollider(), entityB->getPosition());
	}
	// Rectangle with circle
	else if (colliderB->shape == Collider::Circle &&
			 colliderA->shape == Collider::Rectangle) {
		return _collisionCircleRectangle(
			entityB->getCollider(), entityB->getPosition(), colliderA, posA);
	}
	return false;
}

bool GameEngine::_collisionCircleRectangle(
	const Collider *circleCollider, const glm::vec3 &circlePos,
	const Collider *rectangleCollider, const glm::vec3 &rectanglePos) const {
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

bool GameEngine::_tryShortcut(
	Entity *entity, glm::vec3 &futureMovement, glm::vec3 &shortcutMovement,
	Entity *toAvoid, std::vector<Entity *> &possibleCollisionEntities) {
	size_t idxOfCollision;

	float absX = abs(futureMovement.x);
	float absZ = abs(futureMovement.z);
	// Try moving indipendently one of the two axis
	// (Only if its a two-axis move)
	if (futureMovement.x != 0.0f && futureMovement.z != 0.0f) {
		shortcutMovement.x = futureMovement.x;
		shortcutMovement.z = 0.0f;
		idxOfCollision = _checkCollision(entity, shortcutMovement,
										 possibleCollisionEntities);
		if (idxOfCollision == possibleCollisionEntities.size()) {
			return true;
		}
		shortcutMovement.x = 0.0f;
		shortcutMovement.z = futureMovement.z;
		idxOfCollision = _checkCollision(entity, shortcutMovement,
										 possibleCollisionEntities);
		if (idxOfCollision == possibleCollisionEntities.size()) {
			return true;
		}
	}

	// Special cases for Circle, will be able to
	// "circle around" obstacles more easily if more
	// than half is outside collision
	if (entity->getCollider()->shape == Collider::Circle) {
		// Only try to move along the most important
		// targetMovement
		if (absX >= absZ) {
			// Try slide under obstacle
			float colliderZ =
				toAvoid->getPosition().z + toAvoid->getCollider()->height;
			if (entity->getPosition().z > colliderZ) {
				shortcutMovement.z =
					colliderZ -
					(entity->getPosition().z - entity->getCollider()->height);
				if (shortcutMovement.z > (absX / 2)) {
					shortcutMovement.z = absX / 2;
				}
				if (shortcutMovement.z < 0.0f)
					shortcutMovement.z = EPSILON;
				else if (shortcutMovement.z < EPSILON)
					shortcutMovement.z += EPSILON;
				// Move along X will be less than Z
				// so that no collision will occur
				shortcutMovement.x = shortcutMovement.z * 0.5f;
				if (futureMovement.x < 0) shortcutMovement.x *= -1.0f;
				if (_checkCollision(entity, shortcutMovement,
									possibleCollisionEntities) ==
					possibleCollisionEntities.size()) {
					return true;
				}
			}
			// Try slide over obstacle
			colliderZ -= 2.0f * toAvoid->getCollider()->height;
			if (entity->getPosition().z < colliderZ) {
				shortcutMovement.z =
					(entity->getPosition().z + entity->getCollider()->height) -
					colliderZ;
				if (shortcutMovement.z > (absX / 2)) {
					shortcutMovement.z = absX / 2;
				}
				if (shortcutMovement.z < 0.0f)
					shortcutMovement.z = EPSILON;
				else if (shortcutMovement.z < EPSILON)
					shortcutMovement.z += EPSILON;
				// Move along X will be less than Z
				// so that no collision will occur
				shortcutMovement.x = shortcutMovement.z * 0.5f;
				if (futureMovement.x < 0) shortcutMovement.x *= -1.0f;
				shortcutMovement.z *= -1;
				idxOfCollision = _checkCollision(entity, shortcutMovement,
												 possibleCollisionEntities);
				if (idxOfCollision == possibleCollisionEntities.size()) {
					return true;
				}
			}
		}
		if (absZ >= absX) {
			// Try slide right of obstacle
			float colliderX =
				toAvoid->getPosition().x + toAvoid->getCollider()->width;
			if (entity->getPosition().x > colliderX) {
				shortcutMovement.x = colliderX - (entity->getPosition().x -
												  entity->getCollider()->width);
				if (shortcutMovement.x > (absZ / 2)) {
					shortcutMovement.x = absZ / 2;
				}
				if (shortcutMovement.x < 0.0f)
					shortcutMovement.x = EPSILON;
				else if (shortcutMovement.x < EPSILON)
					shortcutMovement.x += EPSILON;
				// Move along Z will be less than X
				// so that no collision will occur
				shortcutMovement.z = shortcutMovement.x * 0.5f;
				if (futureMovement.z < 0) shortcutMovement.z *= -1.0f;
				idxOfCollision = _checkCollision(entity, shortcutMovement,
												 possibleCollisionEntities);
				if (idxOfCollision == possibleCollisionEntities.size()) {
					return true;
				}
			}
			// Try slide over obstacle
			colliderX -= 2.0f * toAvoid->getCollider()->width;
			if (entity->getPosition().x < colliderX) {
				shortcutMovement.x =
					(entity->getPosition().x + entity->getCollider()->width) -
					colliderX;
				if (shortcutMovement.x > (absZ / 2)) {
					shortcutMovement.x = absZ / 2;
				}
				if (shortcutMovement.x < 0.0f)
					shortcutMovement.x = EPSILON;
				else if (shortcutMovement.x < EPSILON)
					shortcutMovement.x += EPSILON;
				// Move along X will be less than Z
				// so that no collision will occur
				shortcutMovement.z = shortcutMovement.x * 0.5f;
				if (futureMovement.z < 0) shortcutMovement.z *= -1.0f;
				shortcutMovement.x *= -1;
				idxOfCollision = _checkCollision(entity, shortcutMovement,
												 possibleCollisionEntities);
				if (idxOfCollision == possibleCollisionEntities.size()) {
					return true;
				}
			}
		}
	}
	return false;
}

void GameEngine::_setNewResolution() {
	_gameRenderer->setNewResolution(_game->isFullScreen(),
									_game->getWindowWidth(),
									_game->getWindowHeight());
	_camera->configGUI(_gameRenderer->getGUI());
	for (auto entity : _allEntities) {
		entity->updateModel();
	}
	_game->needResolutionChange = false;
}

void GameEngine::updateMusicVolume(int newValue) {
	_audioManager->updateMusicVolume(newValue);
}
void GameEngine::updateSoundsVolume(int newValue) {
	_audioManager->updateSoundsVolume(newValue);
}

void GameEngine::buttonStateChanged(int keyID, bool isPressed) {
	if (_keyboardMap.find(keyID) == _keyboardMap.end()) {
		KeyState keyState;
		_keyboardMap[keyID] = keyState;
	}
	_keyboardMap[keyID].currFrame = isPressed;
}

bool GameEngine::isKeyPressed(int keyID) {
	auto result = _keyboardMap.find(keyID);
	return result != _keyboardMap.end() && result->second.currFrame;
}

bool GameEngine::isKeyJustPressed(int keyID) {
	auto result = _keyboardMap.find(keyID);
	return result != _keyboardMap.end() && result->second.currFrame &&
		   !result->second.prevFrame;
}

void GameEngine::playMusic(std::string musicPath) {
	_audioManager->playMusic(musicPath);
}
void GameEngine::playSound(std::string soundName) {
	_audioManager->playSound(soundName);
}

std::map<int, KeyState> GameEngine::_keyboardMap = std::map<int, KeyState>();
