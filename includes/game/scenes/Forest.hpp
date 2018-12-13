#pragma once

#include "game/scenes/SceneTools.hpp"

class Forest : public SceneTools {
   public:
	Forest(WorldLocation &dialogueLocation, WorldLocation &gameplayLocation,
		   float transitionTime, Bomberman *bomberman);
	virtual ~Forest(void);

	virtual void configGUI(GUI *graphicUI);
	virtual void configAI(void);
	virtual void tellPosition(Entity *entity);
	virtual void update(void);

   private:
	Forest(void);
	Forest(Forest const &src);
	Forest &operator=(Forest const &rhs);

	float _cooldown;
};
