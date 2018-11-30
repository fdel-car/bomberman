#pragma once

#include "game/scenes/SceneTools.hpp"

class Desert : public SceneTools {
   public:
	Desert(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
		   Bomberman *bomberman);
	virtual ~Desert(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);
	virtual void configAI(void);
	virtual void tellPosition(Entity *entity);
	virtual void update(void);

	// void printMapInfo(void);

   private:
	Desert(void);
	Desert(Desert const &src);
	Desert &operator=(Desert const &rhs);

	float _timer;
	float _cooldown;
	bool _pauseMenu;
};
