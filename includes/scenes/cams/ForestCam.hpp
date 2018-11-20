#pragma once

#include "Camera.hpp"

class ForestCam : public Camera {
   public:
	ForestCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
	virtual ~ForestCam(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);

   private:
	ForestCam(void);
	ForestCam(ForestCam const &src);
	ForestCam &operator=(ForestCam const &rhs);

	void _displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
						  int *startStrIdx, std::string name,
						  std::string imgName, std::string text, bool isImgLeft,
						  size_t maxCharPerLine, int nbrOfLine,
						  nk_flags textPosition, std::string fontText,
						  std::string fontTitle);

	bool _pauseMenu;
	bool _slowDialogue;
};