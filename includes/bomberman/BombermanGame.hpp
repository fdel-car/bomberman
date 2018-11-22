#pragma once

#include "bomberman/Save.hpp"
#include "engine/AGame.hpp"

class BombermanGame : public AGame {
	typedef void (BombermanGame::*Scene)(void);

   public:
	BombermanGame(void);
	virtual ~BombermanGame(void);

	virtual bool loadSceneByIndex(int sceneIdx);
	virtual size_t getWindowWidth();
	virtual size_t getWindowHeight();
	virtual bool isFullScreen();

   private:
	Save save;
	std::map<std::string, Scene> _scenesMap;

	void _initScenes(void);

	void _mainMenu(void);
	void _forest(void);
};