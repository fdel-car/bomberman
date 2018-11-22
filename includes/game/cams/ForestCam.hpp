#pragma once

#include "game/cams/Tools.hpp"

class ForestCam : public Tools {
   public:
	ForestCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
	virtual ~ForestCam(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);
	virtual void tellPosition(Entity *entity);

	// void printMapInfo(void);

   private:
	ForestCam(void);
	ForestCam(ForestCam const &src);
	ForestCam &operator=(ForestCam const &rhs);

	bool _pauseMenu;
};
