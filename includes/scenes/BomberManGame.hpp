#pragma once

#include "AGame.hpp"

class BomberManGame : public AGame {

	public:
		BomberManGame(void);
		virtual ~BomberManGame(void);
		virtual bool loadScene(size_t sceneIdx);
};
