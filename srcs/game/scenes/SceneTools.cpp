#include "game/scenes/SceneTools.hpp"
#include "engine/GameEngine.hpp"
#include "game/entities/Bomb.hpp"
#include "game/entities/Explosion.hpp"

SceneTools::SceneTools(size_t mapWidth, size_t mapHeight, glm::vec3 const &pos,
					   glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles),
	  _slowGUIAnimation(true),
	  _mapWidth(mapWidth),
	  _mapHeight(mapHeight),
	  _entitiesInSquares(
		  std::vector<std::map<size_t, Entity *>>(mapWidth * mapHeight)),
	  _xOffset(static_cast<float>(_mapWidth) / 2.0f),
	  _zOffset(static_cast<float>(_mapHeight) / 2.0f) {}

SceneTools::~SceneTools(void) { _clearGraphe(); }

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

bool SceneTools::_displayPauseMenu(GUI *graphicUI, int *_newSceneIdx,
								   int restartIdx, int leaveIdx) {
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
		if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
								"Restart level", "", "14_BOMBERMAN")) {
			*_newSceneIdx = restartIdx;
			res = false;
		}
		if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
								"Quit level", "", "14_BOMBERMAN")) {
			*_newSceneIdx = leaveIdx;
			res = false;
		}
		if (graphicUI->uiButton(0, 0, 0, "", "", "")) {
		}
	}
	graphicUI->uiEndBlock();
	return res;
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

void SceneTools::configAI(void) { return ; }

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
	if (entity->getTag().compare("Player") == 0) _playerPos = vectorIdx;

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
	std::cout << "-------------------------------------------" << std::endl;
	size_t i = 0;
	for (const auto &info : _entitiesInSquares) {
		if (i % _mapWidth != 0) std::cout << " ";
		if (info.empty())
			std::cout << "0";
		else
			std::cout << info.size();
		i++;
		if (i % _mapHeight == 0) std::cout << std::endl;
	}
}

bool SceneTools::putBomb(float xCenter, float zCenter, float explosionTimer,
						 size_t range) {
	size_t xCoord = static_cast<size_t>(xCenter + _xOffset);
	size_t zCoord = static_cast<size_t>(zCenter + _zOffset);
	bool canPutBomb = true;
	for (auto entity : _entitiesInSquares[zCoord * _mapWidth + xCoord]) {
		if (entity.second->getTag().compare("Bomb") == 0 ||
			entity.second->getTag().compare("Wall") == 0 ||
			entity.second->getTag().compare("Box") == 0) {
			canPutBomb = false;
			break;
		}
	}
	// Add bomb to entities
	if (canPutBomb) {
		_gameEngine->addNewEntity(new Bomb(
			glm::vec3(xCoord - _xOffset + 0.5f, 0.0f, zCoord - _zOffset + 0.5f),
			explosionTimer, range, this));
	}
	return canPutBomb;
}

void SceneTools::putExplosion(float xCenter, float zCenter, size_t range) {
	size_t xCoord = static_cast<size_t>(xCenter + _xOffset);
	size_t zCoord = static_cast<size_t>(zCenter + _zOffset);

	// Center explosion
	_gameEngine->addNewEntity(new Explosion(
		glm::vec3(xCoord - _xOffset + 0.5f, 0.0f, zCoord - _zOffset + 0.5f),
		this));
	// Left explosion
	_putExplosionsInDirection(xCoord, zCoord, -1, 0, range);
	// Right explosion
	_putExplosionsInDirection(xCoord, zCoord, 1, 0, range);
	// Top explosion
	_putExplosionsInDirection(xCoord, zCoord, 0, -1, range);
	// Down explosion
	_putExplosionsInDirection(xCoord, zCoord, 0, 1, range);
}

void SceneTools::_putExplosionsInDirection(size_t xCoord, size_t zCoord,
										   int xChange, int zChange,
										   size_t range) {
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
				break;
			} else if (entity.second->getTag().compare("Box") == 0) {
				// Force stop when destroying first Box
				rangeIdx = range;
			} else if (entity.second->getTag().compare("Bomb") == 0) {
				// TODO: one explosion trigger other bombs to explode ?
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

void SceneTools::_startBuildingGrapheForPathFinding(void) {
	// Clear the old graphe
	_clearGraphe();
	// std::cout << "Pos player "<< _playerPos << std::endl;
	// Get coord of the player
	size_t x = _playerPos % _mapWidth;
	size_t z = _playerPos / _mapHeight;
	size_t pos;
	size_t dist = 0;

	// Create the first node who will be the target for the enemies with the
	// player position
	Node *originNode = new Node(nullptr, dist, x, z, _playerPos);
	_graphe.insert(std::pair<size_t, Node *>(_playerPos, originNode));

	// Build the graphe depth by depth
	std::list<Node *> nodesByDepth;
	nodesByDepth.push_back(originNode);

	while (nodesByDepth.size() > 0) {
		// _describeNode(nodesByDepth.front());
		x = nodesByDepth.front()->x;
		z = nodesByDepth.front()->z;
		if (dist == nodesByDepth.front()->dist) dist++;
		if (x > 1) {
			pos = z * _mapHeight + (x - 1);
			_buildNewNode(dist, x - 1, z, pos, nodesByDepth.front(),
						  &nodesByDepth);
		}
		if (x < _mapWidth - 1) {
			pos = z * _mapHeight + (x + 1);
			_buildNewNode(dist, x + 1, z, pos, nodesByDepth.front(),
						  &nodesByDepth);
		}
		if (z > 1) {
			pos = (z - 1) * _mapHeight + x;
			_buildNewNode(dist, x, z - 1, pos, nodesByDepth.front(),
						  &nodesByDepth);
		}
		if (z < _mapHeight - 1) {
			pos = (z + 1) * _mapHeight + x;
			_buildNewNode(dist, x, z + 1, pos, nodesByDepth.front(),
						  &nodesByDepth);
		}
		nodesByDepth.pop_front();
	}
}

void SceneTools::_buildNewNode(size_t dist, size_t x, size_t z, size_t pos,
							   Node *node, std::list<Node *> *nodesByDepth) {
	for (const auto &entity : _entitiesInSquares[pos]) {
		for (const auto &decor : _staticDecor) {
			if (entity.second->getTag().compare(decor) == 0)
				return ;
		}
	}
	if (_graphe.find(pos) == _graphe.end()) {
		// Save if it's a new node
		Node *tmpNode = new Node(node, dist, x, z, pos);
		_graphe.insert(std::pair<size_t, Node *>(pos, tmpNode));
		nodesByDepth->push_back(tmpNode);
	} else  // Save the change if the node exist
		_graphe.at(pos)->updateNode(node, dist);
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

// Soon in a new file

Node::Node() {}

Node::Node(Node *newPrev, size_t newDist, size_t xPos, size_t zPos,
		   size_t newId)
	: dist(newDist), x(xPos), z(zPos), id(newId) {
	if (newPrev != nullptr) {
		std::vector<Node *> tmpVector;
		tmpVector.push_back(newPrev);
		prevNodesByDist.insert(
			std::pair<size_t, std::vector<Node *>>(newDist, tmpVector));
	}
}

Node::~Node(void) {}

void Node::updateNode(Node *old, size_t dist) {
	if (prevNodesByDist.find(dist) == prevNodesByDist.end()) {
		std::vector<Node *> tmpVector;
		tmpVector.push_back(old);
		prevNodesByDist.insert(
			std::pair<size_t, std::vector<Node *>>(dist, tmpVector));
	} else
		prevNodesByDist[dist].push_back(old);
}
