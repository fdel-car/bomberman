#pragma once

#include "AGame.hpp"

class BombermanGame : public AGame {
   public:
	BombermanGame(void);
	virtual ~BombermanGame(void);
	virtual bool loadScene(size_t sceneIdx);
};
