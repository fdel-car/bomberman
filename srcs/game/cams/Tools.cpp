#include "game/cams/Tools.hpp"
#include "engine/GameEngine.hpp"
#include "game/entities/Bomb.hpp"

Tools::Tools(size_t mapWidth, size_t mapHeight, glm::vec3 const &pos,
			 glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles),
	  _slowGUIAnimation(true),
	  _mapWidth(mapWidth),
	  _mapHeight(mapHeight),
	  _entitiesInSquares(
		  std::vector<std::map<size_t, Entity *>>(mapWidth * mapHeight)),
	  _xOffset(static_cast<float>(_mapWidth) / 2.0f),
	  _zOffset(static_cast<float>(_mapHeight) / 2.0f) {}

Tools::~Tools(void) {}

void Tools::_displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
							 int *startStrIdx, std::string name,
							 std::string imgName, std::string str,
							 bool isImgLeft, size_t maxCharPerLine,
							 int nbrOfLine, nk_flags textPosition,
							 std::string fontText, std::string fontTitle) {
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

bool Tools::_displayPauseMenu(GUI *graphicUI, int *_newSceneIdx, int restartIdx,
							  int leaveIdx) {
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

bool Tools::_btnHover(GUI *graphicUI, int rectWidth, int rectHeight,
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

void Tools::tellPosition(Entity *entity) { _savePositions(entity); }

void Tools::tellDestruction(Entity *entity) {
	// Clear entity data
	if (_entitiesInfos.find(entity->getId()) != _entitiesInfos.end()) {
		for (auto savedIdx : _entitiesInfos[entity->getId()]) {
			_entitiesInSquares[savedIdx].erase(entity->getId());
		}
		_entitiesInfos.erase(entity->getId());
	}
}

void Tools::_savePositions(Entity *entity) {
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

void Tools::printMapInfo(void) {
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

bool Tools::putBomb(float xCenter, float zCenter, float explosionTimer,
					size_t range) {
	size_t xCoord = static_cast<size_t>(xCenter + _xOffset);
	size_t zCoord = static_cast<size_t>(zCenter + _zOffset);
	bool canPutBomb = true;
	for (auto entity : _entitiesInSquares[zCoord * _mapWidth + xCoord]) {
		if (entity.second->getTag().compare("Bomb") == 0) {
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

void Tools::putExplosion(float xCenter, float zCenter, size_t range) {
	(void)xCenter;
	(void)zCenter;
	(void)range;
}

std::map<size_t, std::vector<size_t>> const &Tools::getEntitiesInfos() const {
	return _entitiesInfos;
}

std::vector<std::map<size_t, Entity *>> const &Tools::getEntitiesInSquares()
	const {
	return _entitiesInSquares;
}

size_t const &Tools::getMapWidth() const { return _mapWidth; }

size_t const &Tools::getMapHeight() const { return _mapHeight; }


void Tools::_pathFinding(void) {
	// for (size_t i = 0; i < _entitiesInSquares.size(); i++) {
	//
	// }
}
