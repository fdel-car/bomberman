#pragma once

#include "game/scenes/SceneTools.hpp"

class Space : public SceneTools {
   public:
	Space(WorldLocation &dialogueLocation, WorldLocation &gameplayLocation,
		  float transitionTime, Bomberman *bomberman);
	virtual ~Space(void);

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
