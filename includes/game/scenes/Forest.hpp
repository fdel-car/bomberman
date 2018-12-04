#pragma once

#include "game/scenes/SceneTools.hpp"

class Forest : public SceneTools {
   public:
	Forest(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
		   Bomberman *bomberman);
	virtual ~Forest(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);
	virtual void configAI(void);
	virtual void tellPosition(Entity *entity);
	virtual void update(void);

	// void printMapInfo(void);

   private:
	Forest(void);
	Forest(Forest const &src);
	Forest &operator=(Forest const &rhs);

	std::vector<Dialogue> _dialogues;
	float _cooldown;
};
