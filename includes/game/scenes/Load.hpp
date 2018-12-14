#pragma once

#include "engine/GameEngine.hpp"
#include "game/scenes/SceneTools.hpp"

class Load : public SceneTools {
   public:
	Load(WorldLocation &startLocation, Bomberman *bomberman);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);

   private:
	Load(void);
	virtual ~Load(void);
	Load(Load const &src);

	Load &operator=(Load const &rhs);
};
