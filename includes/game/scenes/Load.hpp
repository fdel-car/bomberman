#pragma once

#include "engine/GameEngine.hpp"
#include "game/scenes/SceneTools.hpp"

class Load : public SceneTools {
   public:
	Load(WorldLocation &startLocation, Bomberman *bomberman);
	Load(Load const &src);

	Load &operator=(Load const &rhs);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);

   private:
	Load(void);
	virtual ~Load(void);
};
