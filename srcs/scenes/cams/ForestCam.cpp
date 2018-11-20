#include "scenes/cams/ForestCam.hpp"
#include "GameEngine.hpp"

ForestCam::ForestCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles),
	  _entitiesInSquares(
		  std::vector<std::map<size_t, Entity *>>(mapWidth * mapHeight)) {}

ForestCam::~ForestCam(void) {}

void ForestCam::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(_neededImages);

	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;
	_pauseMenu = false;
	_slowDialogue = false;
}

void ForestCam::drawGUI(GUI *graphicUI) {
	if (_pauseMenu || _gameEngine->isKeyPressed("E")) {
		_pauseMenu = true;
		if (graphicUI->uiStartBlock(
				"PauseMenu", "Pause",
				nk_rect((WINDOW_W / 2) - (WINDOW_W / 8), (WINDOW_H / 3),
						WINDOW_W / 4, WINDOW_H / 3),
				NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
			if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
									"Resume", "", "14_BOMBERMAN")) {
				_pauseMenu = false;
			}
			if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
									"Restart level", "", "14_BOMBERMAN")) {
				_newSceneIdx = 1;
			}
			if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
									"Quit level", "", "14_BOMBERMAN")) {
				_newSceneIdx = 0;
			}
			if (graphicUI->uiButton(0, 0, 0, "", "", "")) {
			}
		}
		graphicUI->uiEndBlock();
	}
	static int searchWord = 0;
	static int lastWord = 0;
	static int startStrIdx = 0;
	std::string str =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
		"officia deserunt mollit anim id est laborum.";
	size_t maxCharPerLine =
		((WINDOW_W / 4) * 3 - (((WINDOW_H / 4) - 45) - (WINDOW_W / 4)) - 40) /
		8.5;
	int nbrOfLine = (((WINDOW_H / 4) - 45) / 22) - 2;
	_displayDialogue(graphicUI, &searchWord, &lastWord, &startStrIdx,
					 "Bomberman", "", str, false, maxCharPerLine, nbrOfLine,
					 NK_TEXT_LEFT, "12_BOMBERMAN", "18_BOMBERMAN");
}

void ForestCam::_displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
								 int *startStrIdx, std::string name,
								 std::string imgName, std::string str,
								 bool isImgLeft, size_t maxCharPerLine,
								 int nbrOfLine, nk_flags textPosition,
								 std::string fontText, std::string fontTitle) {
	if (_slowDialogue) {
		if (*searchWord < (int)str.size()) {
			*searchWord += 1;
			if (str[*searchWord] == ' ') *lastWord = *searchWord;
		} else
			*lastWord = *searchWord;
		if (*lastWord - *startStrIdx >= (int)maxCharPerLine * (nbrOfLine - 1))
			*startStrIdx += maxCharPerLine;
	}
	_slowDialogue = !_slowDialogue;
	std::string displayableStr =
		str.substr(*startStrIdx, *lastWord - *startStrIdx);
	graphicUI->uiDialogBox(name.c_str(), imgName, displayableStr.c_str(),
						   isImgLeft, maxCharPerLine, nbrOfLine, textPosition,
						   fontText, fontTitle);
}

void ForestCam::tellPosition(Entity *entity) {
	// if (_entitiesInfos.find(entity->getId()) != _entitiesInfos.end()) {
	std::vector<size_t> allNewSquareWeAreIn;

	// Get these to be faster later
	const glm::vec3 &entityPos = entity->getPosition();
	const Collider *entityCol = entity->getCollider();

	// TopLeft
	size_t xCoord =
		static_cast<size_t>(entityPos.x - entityCol->width + 0.0001f + xOffset);
	size_t zCoord = static_cast<size_t>(entityPos.z - entityCol->height +
										0.0001f + zOffset);
	size_t vectorIdx = zCoord * mapWidth + xCoord;
	allNewSquareWeAreIn.push_back(vectorIdx);
	// TopRight
	xCoord =
		static_cast<size_t>(entityPos.x + entityCol->width - 0.0001f + xOffset);
	zCoord = static_cast<size_t>(entityPos.z - entityCol->height + 0.0001f +
								 zOffset);
	vectorIdx = zCoord * mapWidth + xCoord;
	if (allNewSquareWeAreIn.front() != vectorIdx)
		allNewSquareWeAreIn.push_back(vectorIdx);
	// Bottom-Left
	xCoord =
		static_cast<size_t>(entityPos.x - entityCol->width + 0.0001f + xOffset);
	zCoord = static_cast<size_t>(entityPos.z + entityCol->height - 0.0001f +
								 zOffset);
	vectorIdx = zCoord * mapWidth + xCoord;
	if (allNewSquareWeAreIn.front() != vectorIdx &&
		allNewSquareWeAreIn.back() != vectorIdx)
		allNewSquareWeAreIn.push_back(vectorIdx);
	// Bottom-Right
	xCoord =
		static_cast<size_t>(entityPos.x + entityCol->width - 0.0001f + xOffset);
	zCoord = static_cast<size_t>(entityPos.z + entityCol->height - 0.0001f +
								 zOffset);
	vectorIdx = zCoord * mapWidth + xCoord;
	if (std::find(allNewSquareWeAreIn.begin(), allNewSquareWeAreIn.end(),
				  vectorIdx) == allNewSquareWeAreIn.end())
		allNewSquareWeAreIn.push_back(vectorIdx);

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
	// }

	// std::cout << *entity << std::endl;
}

void ForestCam::printMapInfo(void) {
	std::cout << "-------------------------------------------" << std::endl;
	size_t i = 0;
	for (const auto &info : _entitiesInSquares) {
		if (i % mapWidth != 0) std::cout << " ";
		if (info.empty())
			std::cout << "0";
		else
			std::cout << "1";
		i++;
		if (i % mapHeight == 0) std::cout << std::endl;
	}
}