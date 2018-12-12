#pragma once

#include "game/scenes/SceneTools.hpp"

class Pokemon : public SceneTools {
   public:
	Pokemon(WorldLocation &dialogueLocation, WorldLocation &gameplayLocation,
			float transitionTime, Bomberman *bomberman);
	virtual ~Pokemon(void);

	virtual void configGUI(GUI *graphicUI);
	virtual void configAI(void);
	virtual void tellPosition(Entity *entity);
	virtual void update(void);

	// void printMapInfo(void);

   private:
	Pokemon(void);
	Pokemon(Pokemon const &src);
	Pokemon &operator=(Pokemon const &rhs);

	std::vector<Dialogue> _dialogues;
	float _cooldown;
};
