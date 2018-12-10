#pragma once

#include "game/scenes/SceneTools.hpp"

class Space : public SceneTools {
   public:
	Space(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
		  Bomberman *bomberman);
	virtual ~Space(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);
	virtual void configAI(void);
	virtual void tellPosition(Entity *entity);
	virtual void update(void);

	// void printMapInfo(void);

   private:
	Space(void);
	Space(Space const &src);
	Space &operator=(Space const &rhs);

	float _cooldown;
};
