#pragma once

#include "Camera.hpp"
#include "scenes/GUITools.hpp"

class ForestCam : public Camera, public GUITools {
   public:
	ForestCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
	virtual ~ForestCam(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);
	virtual void tellPosition(Entity *entity);

	void printMapInfo(void);

   private:
	ForestCam(void);
	ForestCam(ForestCam const &src);
	ForestCam &operator=(ForestCam const &rhs);

	// void _displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
	// 					  int *startStrIdx, std::string name,
	// 					  std::string imgName, std::string text, bool isImgLeft,
	// 					  size_t maxCharPerLine, int nbrOfLine,
	// 					  nk_flags textPosition, std::string fontText,
	// 					  std::string fontTitle);

	bool _pauseMenu;
	size_t mapWidth = 17;
	size_t mapHeight = 17;
	float xOffset = 8.5f;
	float zOffset = 8.5f;
	std::map<size_t, std::vector<size_t>> _entitiesInfos;
	std::vector<std::map<size_t, Entity *>> _entitiesInSquares;
};
