#pragma once

#include "game/scenes/SceneTools.hpp"

class Mario : public SceneTools {
   public:
	Mario(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
			Bomberman *bomberman);
	virtual ~Mario(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);
	virtual void configAI(void);
	virtual void tellPosition(Entity *entity);
	virtual void update(void);

	// void printMapInfo(void);

   private:
	Mario(void);
	Mario(Mario const &src);
	Mario &operator=(Mario const &rhs);

	float _cooldown;
};
