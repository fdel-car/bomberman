#pragma once

#include "AGame.hpp"

class BombermanGame : public AGame {
   public:
	typedef bool (BombermanGame::*Scene)(void);

	BombermanGame(void);
	virtual ~BombermanGame(void);
	virtual bool loadScene(size_t sceneIdx);

   private:
	static std::vector<Scene> _scenes;

	static std::vector<Scene> _initScenes(void);

	bool _mainMenu(void);
	bool _factory(void);
};
