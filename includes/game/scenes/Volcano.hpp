#pragma once

#include "game/scenes/SceneTools.hpp"

class Volcano : public SceneTools {
   public:
	Volcano(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
			Bomberman *bomberman);
	virtual ~Volcano(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);
	virtual void configAI(void);
	virtual void tellPosition(Entity *entity);
	virtual void update(void);

	// void printMapInfo(void);

   private:
	Volcano(void);
	Volcano(Volcano const &src);
	Volcano &operator=(Volcano const &rhs);

	float _cooldown;
};
