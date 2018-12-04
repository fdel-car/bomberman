#include "game/scenes/SceneTools.hpp"
#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"
#include "game/entities/Bomb.hpp"
#include "game/entities/Damageable.hpp"
#include "game/entities/Explosion.hpp"

extern std::string _assetsDir;

SceneTools::SceneTools(size_t mapWidth, size_t mapHeight, glm::vec3 const &pos,
					   glm::vec3 const &eulerAngles, Bomberman *bomberman,
					   std::string ownLvlName, std::string nextLvlName,
					   float timer)
	: Camera(pos, eulerAngles),
	  _bomberman(bomberman),
	  _save(bomberman->getSave()),
	  _slowGUIAnimation(true),
	  _playerMaxHp(0),
	  _playerHp(0),
	  _showPlayerHp(false),
	  _showVictoryScreen(false),
	  _showDeathScreen(false),
	  _mapWidth(mapWidth),
	  _mapHeight(mapHeight),
	  _entitiesInSquares(
		  std::vector<std::map<size_t, Entity *>>(mapWidth * mapHeight)),
	  _firstPlayerPos(true),
	  _distanceFromPlayer(glm::vec3(0)),
	  _startMusic(""),
	  _xOffset(static_cast<float>(_mapWidth) / 2.0f),
	  _zOffset(static_cast<float>(_mapHeight) / 2.0f),
	  _ownLvlName(ownLvlName),
	  _startLvlName(bomberman->getStartLevelName()),
	  _nextLvlName(nextLvlName),
	  _timer(timer),
	  _pauseMenu(false) {
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/icons/heart.png"), "heart"));
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/icons/sad_chopper.png"), "sad_chopper"));

	_initNeededSounds();
	// TODO: music when starting dialogues
}

SceneTools::~SceneTools(void) { _clearGraphe(); }

void SceneTools::_initNeededSounds(void) {
	// Explosion sounds
	_neededSounds["explosion_1"] =
		_assetsDir + "Audio/Sounds/Explosion/explosion_1.wav";
	_neededSounds["explosion_2"] =
		_assetsDir + "Audio/Sounds/Explosion/explosion_2.wav";
	_neededSounds["explosion_with_box_1"] =
		_assetsDir + "Audio/Sounds/Explosion/explosion_with_box_1.wav";
	_neededSounds["explosion_with_box_2"] =
		_assetsDir + "Audio/Sounds/Explosion/explosion_with_box_2.wav";

	_explosionSounds.push_back("explosion_1");
	_explosionSounds.push_back("explosion_2");
	_explosionWithBoxSounds.push_back("explosion_with_box_1");
	_explosionWithBoxSounds.push_back("explosion_with_box_2");

	// Time over
	_neededSounds["time_over_effect"] =
		_assetsDir + "Audio/Sounds/Hero/time_over_effect.wav";
	_neededSounds["time_over_voice"] =
		_assetsDir + "Audio/Sounds/Hero/time_over_voice.wav";

	// Win
	_neededSounds["winner_effect"] =
		_assetsDir + "Audio/Sounds/Hero/winner_effect.wav";
	_neededSounds["winner_voice"] =
		_assetsDir + "Audio/Sounds/Hero/winner_voice.wav";

	// Sounds that are not used in this class, but will not be loaded otherwise
	_neededSounds["put_bomb_1"] =
		_assetsDir + "Audio/Sounds/Bomb/put_bomb_1.wav";
	_neededSounds["put_bomb_2"] =
		_assetsDir + "Audio/Sounds/Bomb/put_bomb_2.wav";
	_neededSounds["burn_player_1"] =
		_assetsDir + "Audio/Sounds/Explosion/burn_player_1.wav";
	_neededSounds["burn_player_2"] =
		_assetsDir + "Audio/Sounds/Explosion/burn_player_2.wav";
	_neededSounds["get_perk_1"] =
		_assetsDir + "Audio/Sounds/Perk/get_perk_1.wav";
	_neededSounds["get_perk_2"] =
		_assetsDir + "Audio/Sounds/Perk/get_perk_2.wav";
	_neededSounds["get_perk_3"] =
		_assetsDir + "Audio/Sounds/Perk/get_perk_3.wav";
	_neededSounds["get_perk_4"] =
		_assetsDir + "Audio/Sounds/Perk/get_perk_4.wav";
	_neededSounds["portal_spawn"] =
		_assetsDir + "Audio/Sounds/Portal/portal_spawn.wav";
}

void SceneTools::initEntity(GameEngine *gameEngine) {
	Camera::initEntity(gameEngine);
	gameEngine->playMusic(_startMusic);
}

void SceneTools::drawGUI(GUI *graphicUI) {
	if (!_debugMode && (_pauseMenu || _gameEngine->isKeyPressed(KEY_ESCAPE))) {
		_pauseMenu = _displayPauseMenu(graphicUI);
		_isPause = _pauseMenu;
	}

	if (_showPlayerHp) {
		if (_showVictoryScreen) {
			_displayVictoryScreen(graphicUI);
		} else if (_showDeathScreen) {
			_displayDeathScreen(graphicUI);
		}
		_displayPlayerHP(graphicUI, _playerHp);
	}

	bool timerCanChange = static_cast<int>(_timer) > 0;
	if (!isPause() && !_showDeathScreen && !_showVictoryScreen)
		_displayTimer(graphicUI, false);
	else
		_displayTimer(graphicUI, true);
	if (timerCanChange && static_cast<int>(_timer) == 0) {
		_gameEngine->playMusic("");
		_gameEngine->playSound("time_over_effect");
		_gameEngine->playSound("time_over_voice");
		_showDeathScreen = true;
	}
}

bool SceneTools::isPause(void) const {
	return _debugMode || _isPause || _showVictoryScreen || _showDeathScreen;
}

void SceneTools::updateDebugMode(void) {
	if (!_isPause && _gameEngine->isKeyJustPressed(KEY_GRAVE_ACCENT)) {
		_debugMode = !_debugMode;

		if (_debugMode) {
			// Save position and rotation
			_positionPrevDebug = getPosition();
			_eulerAnglesPrevDebug = getEulerAngles();
		} else {
			// Put back to position
			translate(_positionPrevDebug - getPosition());
			float xOffset = _eulerAnglesPrevDebug.x - getEulerAngles().x;
			float yOffset = _eulerAnglesPrevDebug.y - getEulerAngles().y;

			if (glm::epsilonNotEqual(0.0f, xOffset, EPSILON)) rotateX(xOffset);
			if (glm::epsilonNotEqual(0.0f, yOffset, EPSILON)) rotateY(yOffset);

			_updateData();
		}
		// Avoid camera jump on first frame
		_lastMousePos.x = _gameEngine->getGameRenderer()->getMousePos().x;
		_lastMousePos.y = _gameEngine->getGameRenderer()->getMousePos().y;
		_gameEngine->getGameRenderer()->switchCursorMode(_debugMode);
		// TODO: Avoid camera jump if _lastMousePos is too weird.
	}
}

void SceneTools::_displayDialogue(GUI *graphicUI, int *searchWord,
								  int *lastWord, int *startStrIdx,
								  std::string name, std::string imgName,
								  std::string str, bool isImgLeft,
								  size_t maxCharPerLine, int nbrOfLine,
								  nk_flags textPosition, std::string fontText,
								  std::string fontTitle) {
	size_t maxCPL =
		((WINDOW_W / 4) * 3 - (((WINDOW_H / 4) - 45) - (WINDOW_W / 4)) - 40) /
		8.5;
	maxCharPerLine = maxCharPerLine > maxCPL ? maxCPL : maxCharPerLine;
	int nbrOfLineTmp = (((WINDOW_H / 4) - 45) / 22) - 2;
	nbrOfLine = nbrOfLine > nbrOfLineTmp ? nbrOfLineTmp : nbrOfLine;
	if (_slowGUIAnimation) {
		if (*searchWord < (int)str.size()) {
			*searchWord += 1;
			if (str[*searchWord] == ' ') *lastWord = *searchWord;
		} else
			*lastWord = *searchWord;
		if (*lastWord - *startStrIdx >= (int)maxCharPerLine * (nbrOfLine - 1))
			*startStrIdx += maxCharPerLine;
	}
	_slowGUIAnimation = !_slowGUIAnimation;
	std::string displayableStr =
		str.substr(*startStrIdx, *lastWord - *startStrIdx);
	graphicUI->uiDialogBox(name.c_str(), imgName, displayableStr.c_str(),
						   isImgLeft, maxCharPerLine, nbrOfLine, textPosition,
						   fontText, fontTitle);
}

bool SceneTools::_displayPauseMenu(GUI *graphicUI) {
	bool res = true;
	if (graphicUI->uiStartBlock(
			"PauseMenu", "Pause",
			nk_rect((WINDOW_W / 2) - (WINDOW_W / 8), (WINDOW_H / 3),
					WINDOW_W / 4, WINDOW_H / 3),
			NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0, "Resume",
								"", "14_BOMBERMAN")) {
			res = false;
		}
		if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0, "Restart",
								"", "14_BOMBERMAN")) {
			_newSceneName = _ownLvlName;
			res = false;
		}
		if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0, "Quit", "",
								"14_BOMBERMAN")) {
			_newSceneName = _startLvlName;
			res = false;
		}
	}
	graphicUI->uiEndBlock();
	return res;
}

void SceneTools::_displayPlayerHP(GUI *graphicUI, size_t hp) {
	int rowHeight = std::min(WINDOW_H / 12, 50);
	int rowWidth = _playerMaxHp * rowHeight;
	int windowWidth = rowWidth + 26;
	int windowHeight = rowHeight + 10;
	(void)hp;
	// (void)rowHeight;
	activeStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 150);

	graphicUI->setStyle(activeStyle);
	if (graphicUI->uiStartBlock("PlayerHP", "",
								nk_rect(20, 20, windowWidth, windowHeight),
								NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER)) {
		// activeStyle[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 0);
		activeStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 0);

		graphicUI->setStyle(activeStyle);

		int imgWidth = rowHeight;
		if (_playerHp != 0) {
			graphicUI->uiRowMultipleElem(true, rowHeight, _playerMaxHp);
			for (size_t i = 0; i < _playerHp; i++) {
				graphicUI->uiAddElemInRow(imgWidth);
				graphicUI->uiSetImage("heart");
			}
			graphicUI->uiRowMultipleElem(false);
		}
	}
	graphicUI->uiEndBlock();
	activeStyle = defaultStyle;
	graphicUI->setStyle(activeStyle);
}

void SceneTools::_displayVictoryScreen(GUI *graphicUI) {
	int windowWidth = WINDOW_W / 4;
	int windowHeight = WINDOW_H / 3;
	int rowHeight = (windowHeight / 3) - 17;
	// int rowWidth = windowWidth - 10;
	// int blockXPadding = 8;
	// (void)rowWidth;
	// (void)rowHeight;
	// return;
	if (graphicUI->uiStartBlock(
			"VictoryScreen", "Victory !",
			nk_rect((WINDOW_W / 2) - (WINDOW_W / 8), (WINDOW_H / 3),
					windowWidth, windowHeight),
			NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		rowHeight += 10;
		if (graphicUI->uiButton(windowWidth, rowHeight, 0, "Next", "",
								"14_BOMBERMAN")) {
			_newSceneName = _nextLvlName;
		}
		if (graphicUI->uiButton(windowWidth, rowHeight, 0, "Restart", "",
								"14_BOMBERMAN")) {
			_newSceneName = _ownLvlName;
		}
		if (graphicUI->uiButton(windowWidth, rowHeight, 0, "Quit", "",
								"14_BOMBERMAN")) {
			_newSceneName = _startLvlName;
		}
	}
	graphicUI->uiEndBlock();
}

void SceneTools::_displayDeathScreen(GUI *graphicUI) {
	int windowWidth = WINDOW_W / 4;
	int windowHeight = WINDOW_H / 3;
	int rowHeight = (windowHeight / 3) - 17;
	int rowWidth = windowWidth - 10;
	// int blockXPadding = 8;
	// (void)rowWidth;
	// (void)rowHeight;
	// return;
	if (graphicUI->uiStartBlock(
			"DeathScreen", "Defeat !",
			nk_rect((WINDOW_W / 2) - (WINDOW_W / 8), (WINDOW_H / 3),
					windowWidth, windowHeight),
			NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		graphicUI->uiRowMultipleElem(true, rowHeight, 1);
		graphicUI->uiAddElemInRow(rowHeight);
		graphicUI->uiAddElemOffset((rowWidth - rowHeight) / 2);
		graphicUI->uiSetImage("sad_chopper");
		graphicUI->uiRowMultipleElem(false);

		rowHeight += 10;
		if (graphicUI->uiButton(windowWidth, rowHeight, 0, "Restart", "",
								"14_BOMBERMAN")) {
			_newSceneName = _ownLvlName;
		}
		if (graphicUI->uiButton(windowWidth, rowHeight, 0, "Quit", "",
								"14_BOMBERMAN")) {
			_newSceneName = _startLvlName;
		}
	}
	graphicUI->uiEndBlock();
}

void SceneTools::_displayTimer(GUI *graphicUI, bool isPause) {
	if (_timer > 0.0f && !isPause) _timer -= _gameEngine->getDeltaTime();
	activeStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 150);
	graphicUI->setStyle(activeStyle);
	if (graphicUI->uiStartBlock("timer", "",
								nk_rect(WINDOW_W / 7 * 3, 0, WINDOW_W / 7, 60),
								NK_WINDOW_NO_SCROLLBAR | NK_COLOR_BORDER)) {
		int tmpMinutes = _timer / 60;
		std::string minutes = std::to_string(tmpMinutes);
		int tmpSec = static_cast<int>(_timer) % 60;
		std::string sec =
			tmpSec < 10 ? "0" + std::to_string(tmpSec) : std::to_string(tmpSec);
		graphicUI->uiHeader((minutes + " : " + sec).c_str(), NK_TEXT_CENTERED,
							50, "30_BOMBERMAN");
	}
	graphicUI->uiEndBlock();
	activeStyle = defaultStyle;
	graphicUI->setStyle(activeStyle);
}

bool SceneTools::_btnHover(GUI *graphicUI, int rectWidth, int rectHeight,
						   int xRectPos, int yRectPos, int fontSize,
						   std::string fontName, int *extraSize, int maxSize,
						   bool *isButtonHover, std::string btnName,
						   std::string btnImageHover, std::string btnImage) {
	bool ret = false;
	if (*extraSize < maxSize && *isButtonHover)
		*extraSize += 1;
	else if (*extraSize > 0 && !*isButtonHover)
		*extraSize -= 1;
	rectWidth += *extraSize;
	rectHeight += *extraSize;
	xRectPos -= *extraSize / 2;
	yRectPos -= *extraSize / 2;
	fontName = std::to_string(fontSize + *extraSize) + fontName;
	if (graphicUI->uiStartBlock(
			btnName.c_str(), "",
			nk_rect(xRectPos, yRectPos, rectWidth, rectHeight),
			NK_WINDOW_NO_SCROLLBAR)) {
		if (graphicUI->uiHover()) {
			*isButtonHover = true;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, btnName.c_str(),
									btnImageHover.c_str(), fontName)) {
				ret = true;
			}
		} else {
			*isButtonHover = false;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, btnName.c_str(),
									btnImage.c_str(), fontName)) {
				ret = true;
			}
		}
	}
	graphicUI->uiEndBlock();
	return ret;
}

void SceneTools::tellPosition(Entity *entity) { _savePositions(entity); }

void SceneTools::tellDestruction(Entity *entity) {
	// Clear entity data
	if (_entitiesInfos.find(entity->getId()) != _entitiesInfos.end()) {
		for (auto savedIdx : _entitiesInfos[entity->getId()]) {
			_entitiesInSquares[savedIdx].erase(entity->getId());
		}
		_entitiesInfos.erase(entity->getId());
	}
}

void SceneTools::configAI(void) { return; }

void SceneTools::_savePositions(Entity *entity) {
	std::vector<size_t> allNewSquareWeAreIn;

	// Get these to be faster later
	const glm::vec3 &entityPos = entity->getPosition();
	const Collider *entityCol = entity->getCollider();
	size_t xCoord;
	size_t zCoord;
	size_t vectorIdx;

	// If entity has no collider, then just use his center
	if (entityCol == nullptr) {
		xCoord = static_cast<size_t>(entityPos.x + _xOffset);
		zCoord = static_cast<size_t>(entityPos.z + _zOffset);
		vectorIdx = zCoord * _mapWidth + xCoord;
		if (vectorIdx >= allNewSquareWeAreIn.size())
			throw std::runtime_error(
				"An entity located outside of the map boundaries tried to be "
				"pushed inside it.");
		allNewSquareWeAreIn.push_back(vectorIdx);
	} else {
		// TopLeft
		xCoord = static_cast<size_t>(entityPos.x - entityCol->width + 0.0001f +
									 _xOffset);
		zCoord = static_cast<size_t>(entityPos.z - entityCol->height + 0.0001f +
									 _zOffset);
		vectorIdx = zCoord * _mapWidth + xCoord;
		allNewSquareWeAreIn.push_back(vectorIdx);
		// TopRight
		xCoord = static_cast<size_t>(entityPos.x + entityCol->width - 0.0001f +
									 _xOffset);
		zCoord = static_cast<size_t>(entityPos.z - entityCol->height + 0.0001f +
									 _zOffset);
		vectorIdx = zCoord * _mapWidth + xCoord;
		if (allNewSquareWeAreIn.front() != vectorIdx)
			allNewSquareWeAreIn.push_back(vectorIdx);
		// Bottom-Left
		xCoord = static_cast<size_t>(entityPos.x - entityCol->width + 0.0001f +
									 _xOffset);
		zCoord = static_cast<size_t>(entityPos.z + entityCol->height - 0.0001f +
									 _zOffset);
		vectorIdx = zCoord * _mapWidth + xCoord;
		if (allNewSquareWeAreIn.front() != vectorIdx &&
			allNewSquareWeAreIn.back() != vectorIdx)
			allNewSquareWeAreIn.push_back(vectorIdx);
		// Bottom-Right
		xCoord = static_cast<size_t>(entityPos.x + entityCol->width - 0.0001f +
									 _xOffset);
		zCoord = static_cast<size_t>(entityPos.z + entityCol->height - 0.0001f +
									 _zOffset);
		vectorIdx = zCoord * _mapWidth + xCoord;
		if (std::find(allNewSquareWeAreIn.begin(), allNewSquareWeAreIn.end(),
					  vectorIdx) == allNewSquareWeAreIn.end())
			allNewSquareWeAreIn.push_back(vectorIdx);
	}

	// Save Player position
	if (entity->getTag().compare("Player") == 0) {
		// std::cout << xCoord << " " << zCoord << std::endl;
		size_t MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM = 5;
		int FOLLOW_CORRECTION = static_cast<int>(_mapHeight / 2) -
								MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM;  // 3
		// std::cout << "FOLLOW_CORRECTION " << FOLLOW_CORRECTION << std::endl;
		FOLLOW_CORRECTION = 3;
		_playerPos = vectorIdx;
		// Move cam
		if (_firstPlayerPos) {
			_firstPlayerPos = false;
			_distanceFromPlayer = getPosition() - entity->getPosition();
			if (xCoord < MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM ||
				xCoord > _mapWidth - MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM) {
				_distanceFromPlayer.x +=
					(xCoord < MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM)
						? -FOLLOW_CORRECTION
						: FOLLOW_CORRECTION;
			}
			if (zCoord < MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM ||
				zCoord > _mapHeight - MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM) {
				_distanceFromPlayer.z +=
					(zCoord < MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM)
						? -FOLLOW_CORRECTION
						: FOLLOW_CORRECTION;
			}
		} else {
			glm::vec3 targetMove = entity->getPosition() + _distanceFromPlayer;
			glm::vec3 translation = targetMove - getPosition();
			if (xCoord < MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM ||
				xCoord > _mapWidth - MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM) {
				translation.x = 0;
			}
			if (zCoord < MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM ||
				zCoord > _mapHeight - MIN_DISTANCE_FROM_WALL_TO_MOVE_CAM) {
				translation.z = 0;
			}
			if (translation.x != 0 || translation.z != 0) {
				this->translate(translation);
				_updateData();
			}
		}
	}

	// Clear entity old Idx saved in _entitiesInSquares
	for (auto savedIdx : _entitiesInfos[entity->getId()]) {
		if (std::find(allNewSquareWeAreIn.begin(), allNewSquareWeAreIn.end(),
					  savedIdx) == allNewSquareWeAreIn.end()) {
			_entitiesInSquares[savedIdx].erase(entity->getId());
		}
	}

	// Add new Idxs
	for (auto newIdx : allNewSquareWeAreIn) {
		if (std::find(_entitiesInfos[entity->getId()].begin(),
					  _entitiesInfos[entity->getId()].end(),
					  newIdx) == _entitiesInfos[entity->getId()].end()) {
			_entitiesInSquares[newIdx].insert(
				std::pair<size_t, Entity *>(entity->getId(), entity));
		}
	}

	// Write over old infos
	_entitiesInfos[entity->getId()] = allNewSquareWeAreIn;
}

void SceneTools::printMapInfo(void) {
	std::cout << "------------------------------------------- " << _mapWidth
			  << " " << _mapHeight << std::endl;
	size_t i = 0;
	size_t j = 0;
	for (const auto &info : _entitiesInSquares) {
		if (i % _mapWidth != 0) std::cout << " ";
		if (info.empty())
			std::cout << "0";
		else
			std::cout << info.size();
		i++;
		j++;
		if (j == _mapWidth) {
			j = 0;
			std::cout << std::endl;
		}
	}
}

bool SceneTools::canPutBomb(float xCenter, float zCenter) {
	size_t xCoord = static_cast<size_t>(xCenter + _xOffset);
	size_t zCoord = static_cast<size_t>(zCenter + _zOffset);
	bool canPut = true;
	for (auto entity : _entitiesInSquares[zCoord * _mapWidth + xCoord]) {
		if (entity.second->getTag().compare("Bomb") == 0 ||
			entity.second->getTag().compare("Wall") == 0 ||
			entity.second->getTag().compare("Box") == 0) {
			canPut = false;
			break;
		}
	}
	return canPut;
}

bool SceneTools::putBomb(float xCenter, float zCenter, float explosionTimer,
						 size_t range) {
	// Add bomb to entities
	if (canPutBomb(xCenter, zCenter)) {
		size_t xCoord = static_cast<size_t>(xCenter + _xOffset);
		size_t zCoord = static_cast<size_t>(zCenter + _zOffset);
		_gameEngine->addNewEntity(new Bomb(
			glm::vec3(xCoord - _xOffset + 0.5f, 0.0f, zCoord - _zOffset + 0.5f),
			explosionTimer, range, this));
		return true;
	}
	return false;
}

void SceneTools::putExplosion(float xCenter, float zCenter, size_t range) {
	size_t xCoord = static_cast<size_t>(xCenter + _xOffset);
	size_t zCoord = static_cast<size_t>(zCenter + _zOffset);

	// Center
	_gameEngine->addNewEntity(new Explosion(
		glm::vec3(xCoord - _xOffset + 0.5f, 0.0f, zCoord - _zOffset + 0.5f),
		this));
	bool hasDestroyedBox = false;
	// Left
	hasDestroyedBox = _putExplosionsInDirection(xCoord, zCoord, -1, 0, range) ||
					  hasDestroyedBox;
	// Right
	hasDestroyedBox = _putExplosionsInDirection(xCoord, zCoord, 1, 0, range) ||
					  hasDestroyedBox;
	// Top
	hasDestroyedBox = _putExplosionsInDirection(xCoord, zCoord, 0, -1, range) ||
					  hasDestroyedBox;
	// Down
	hasDestroyedBox = _putExplosionsInDirection(xCoord, zCoord, 0, 1, range) ||
					  hasDestroyedBox;

	// Play random sound
	if (hasDestroyedBox) {
		int randIdx = rand() % _explosionWithBoxSounds.size();
		_gameEngine->playSound(_explosionWithBoxSounds[randIdx]);
	} else {
		int randIdx = rand() % _explosionSounds.size();
		_gameEngine->playSound(_explosionSounds[randIdx]);
	}
}

bool SceneTools::_putExplosionsInDirection(size_t xCoord, size_t zCoord,
										   int xChange, int zChange,
										   size_t range) {
	bool hasDestroyedBox = false;
	size_t rangeIdx = 0;
	bool canPutExplosion;
	while (rangeIdx < range) {
		xCoord += xChange;
		zCoord += zChange;
		if (xCoord < 1 || xCoord >= _mapWidth || zCoord < 1 ||
			zCoord >= _mapHeight)
			break;
		canPutExplosion = true;
		for (auto entity : _entitiesInSquares[zCoord * _mapWidth + xCoord]) {
			if (entity.second->getTag().compare("Wall") == 0) {
				canPutExplosion = false;
			} else if (entity.second->getTag().compare("Bomb") == 0) {
				// Force stop when destroying first Box/Bomb
				rangeIdx = range;
			} else if (entity.second->getTag().compare("Box") == 0) {
				hasDestroyedBox = true;
				rangeIdx = range;
				canPutExplosion = false;
				Damageable *damageable =
					dynamic_cast<Damageable *>(entity.second);
				if (damageable != nullptr) {
					damageable->takeDamage();
				}
			}
		}
		if (canPutExplosion) {
			_gameEngine->addNewEntity(
				new Explosion(glm::vec3(xCoord - _xOffset + 0.5f, 0.0f,
										zCoord - _zOffset + 0.5f),
							  this));
		} else
			break;
		rangeIdx++;
	}
	return hasDestroyedBox;
}

void SceneTools::tellPlayerHp(size_t hp) {
	_showPlayerHp = true;
	if (hp > _playerMaxHp) _playerMaxHp = hp;
	_playerHp = hp;
	if (_playerHp == 0) {
		_showDeathScreen = true;
		_gameEngine->playMusic("");
	}
}

void SceneTools::tellLevelSuccess() {
	if (_playerHp != 0) {
		_showVictoryScreen = true;
		_gameEngine->playMusic("");
		_gameEngine->playSound("winner_effect");
		_gameEngine->playSound("winner_voice");
		// Save if it's first time we complete lvl
		size_t levelIdx = _bomberman->getSceneIndexByName(_ownLvlName);
		if (_save.level < levelIdx) {
			_save.level = levelIdx;
			_save.doSave();
		}
	}
}

std::map<size_t, std::vector<size_t>> const &SceneTools::getEntitiesInfos()
	const {
	return _entitiesInfos;
}

std::vector<std::map<size_t, Entity *>> const &
SceneTools::getEntitiesInSquares() const {
	return _entitiesInSquares;
}

std::map<size_t, Node *> const &SceneTools::getGraphe() const {
	return _graphe;
}

bool const &SceneTools::getRefreshAI() const { return _refreshAI; }

size_t const &SceneTools::getMapWidth() const { return _mapWidth; }

size_t const &SceneTools::getMapHeight() const { return _mapHeight; }

size_t const &SceneTools::getPlayerPos() const { return _playerPos; }

size_t const &SceneTools::getRunAwayPos() const { return _runAwayPos; }

void SceneTools::_startBuildingGrapheForPathFinding(void) {
	// Clear the old graphe
	_clearGraphe();

	// Get coord of the player
	size_t x = _playerPos % _mapWidth;
	size_t z = _playerPos / _mapWidth;

	// Create the first node who will be the target for the enemies with the
	// player position
	Node *originNode = new Node(nullptr, 0, x, z, _playerPos);
	_graphe.insert(std::pair<size_t, Node *>(_playerPos, originNode));
	_searchWay(true, originNode, _playerPos);
	size_t dist = 0;
	_runAwayPos = 0;
	for (const auto &node : _graphe) {
		if (dist < node.second->dist) {
			dist = node.second->dist;
			_runAwayPos = node.second->id;
		}
	}
	// std::cout << _runAway << std::endl;
	if (_runAwayPos != 0) _searchWay(false, _graphe[_runAwayPos], _runAwayPos);
}

void SceneTools::_searchWay(bool searchBestWay, Node *originNode,
							size_t playerPos) {
	size_t x = playerPos % _mapWidth;
	size_t z = playerPos / _mapWidth;
	size_t pos;
	size_t dist = 0;
	std::list<Node *> nodesByDepth;
	nodesByDepth.push_back(originNode);
	while (nodesByDepth.size() > 0) {
		x = nodesByDepth.front()->x;
		z = nodesByDepth.front()->z;
		if (dist == nodesByDepth.front()->dist && searchBestWay) dist++;
		if (dist == nodesByDepth.front()->runAwayDist && !searchBestWay) dist++;
		if (x > 1) {
			pos = z * _mapWidth + (x - 1);
			_buildNewNode(dist, x - 1, z, pos, nodesByDepth.front(),
						  &nodesByDepth, searchBestWay);
		}
		if (x < _mapWidth - 1) {
			pos = z * _mapWidth + (x + 1);
			_buildNewNode(dist, x + 1, z, pos, nodesByDepth.front(),
						  &nodesByDepth, searchBestWay);
		}
		if (z > 1) {
			pos = (z - 1) * _mapWidth + x;
			_buildNewNode(dist, x, z - 1, pos, nodesByDepth.front(),
						  &nodesByDepth, searchBestWay);
		}
		if (z < _mapHeight - 1) {
			pos = (z + 1) * _mapWidth + x;
			_buildNewNode(dist, x, z + 1, pos, nodesByDepth.front(),
						  &nodesByDepth, searchBestWay);
		}
		nodesByDepth.pop_front();
	}
}

void SceneTools::_buildNewNode(size_t dist, size_t x, size_t z, size_t pos,
							   Node *node, std::list<Node *> *nodesByDepth,
							   bool saveInPrevious) {
	for (const auto &entity : _entitiesInSquares[pos]) {
		for (const auto &decor : _staticDecor) {
			if (entity.second->getName().compare(decor) == 0) return;
		}
	}
	if (_graphe.find(pos) == _graphe.end()) {
		// Save if it's a new node
		Node *tmpNode = new Node(node, dist, x, z, pos);
		_graphe.insert(std::pair<size_t, Node *>(pos, tmpNode));
		for (const auto &map : _entitiesInSquares[pos]) {
			if (static_cast<size_t>(map.second->getPosition().x + _xOffset) ==
					x &&
				static_cast<size_t>(map.second->getPosition().z + _zOffset) ==
					z) {
				for (const auto &decor : _tmpDecor) {
					if (map.second->getName().compare(decor) == 0) {
						tmpNode->entitiesOnMe.push_back(map.second);
						tmpNode->isAnEntity = true;
					}
				}
			}
		}
		if (!tmpNode->isAnEntity) nodesByDepth->push_back(tmpNode);
	} else {  // Save the change if the node exist
		if (!saveInPrevious && _graphe.at(pos)->runAwayDist == 0) {
			bool isPlayer = false;
			for (const auto &entity : _graphe.at(pos)->entitiesOnMe) {
				if (entity->getName().compare("Player") == 0) isPlayer = true;
				for (const auto &vec : _tmpDecor) {
					if (vec.compare(entity->getName()) == 0) isPlayer = true;
				}
			}
			if (!isPlayer) nodesByDepth->push_back(_graphe.at(pos));
		}
		_graphe.at(pos)->updateNode(node, dist, saveInPrevious);
	}
}

void SceneTools::_clearGraphe(void) {
	for (auto node : _graphe) {
		delete node.second;
	}
	_graphe.clear();
}

void SceneTools::_describeNode(Node *n) {
	std::cout << "x    : " << n->x << std::endl;
	std::cout << "z    : " << n->z << std::endl;
	std::cout << "id   : " << n->id << std::endl;
	std::cout << "dist : " << n->dist << std::endl;
	for (const auto &tmp : n->prevNodesByDist) {
		std::cout << tmp.first << " = " << tmp.second.size() << std::endl;
		for (auto tmp2 : tmp.second) {
			std::cout << tmp2->x << std::endl;
			std::cout << tmp2->z << std::endl;
			std::cout << tmp2->id << std::endl;
			std::cout << tmp2->dist << std::endl;
		}
	}
}

Node::Node() {}

Node::Node(Node *newPrev, size_t newDist, size_t xPos, size_t zPos,
		   size_t newId)
	: dist(newDist),
	  runAwayDist(0),
	  x(xPos),
	  z(zPos),
	  id(newId),
	  isFatal(false),
	  isAnEntity(false) {
	if (newPrev == nullptr) return;
	std::vector<Node *> tmpVector;
	tmpVector.push_back(newPrev);
	prevNodesByDist.insert(
		std::pair<size_t, std::vector<Node *>>(dist, tmpVector));
}

Node::~Node(void) {}

void Node::updateNode(Node *old, size_t newDist, bool saveInPrevious) {
	if (prevNodesByDist.find(newDist) == prevNodesByDist.end() &&
		saveInPrevious) {
		std::vector<Node *> tmpVector;
		tmpVector.push_back(old);
		prevNodesByDist.insert(
			std::pair<size_t, std::vector<Node *>>(newDist, tmpVector));
	} else if (saveInPrevious)
		prevNodesByDist[newDist].push_back(old);
	else {
		runAwayDist = newDist;
		if (runAwayNodesByDist.find(runAwayDist) == runAwayNodesByDist.end()) {
			std::vector<Node *> tmpVector;
			tmpVector.push_back(old);
			runAwayNodesByDist.insert(
				std::pair<size_t, std::vector<Node *>>(runAwayDist, tmpVector));
		} else
			runAwayNodesByDist[runAwayDist].push_back(old);
	}
}
