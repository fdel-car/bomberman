#pragma once

#include "AGame.hpp"

class BombermanGame : public AGame {
	typedef void (BombermanGame::*Scene)(void);

   public:
	BombermanGame(void);
	virtual ~BombermanGame(void);
	virtual bool loadSceneByIndex(int sceneIdx);

   private:
	std::map<std::string, Scene> _scenesMap;
	void _initScenes(void);

	void _mainMenu(void);
	void _forest(void);
};
