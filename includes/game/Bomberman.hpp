#pragma once

#include "engine/AGame.hpp"
#include "game/Save.hpp"

// For Threads
#include <atomic>

#define LOAD_IDLE 0
#define LOAD_STARTED 1
#define LOAD_FINISHED 2

enum LayerTag {
	WallLayer = 0,
	BoxLayer,
	PlayerLayer,
	PlayerSpecialLayer,
	EnemyLayer,
	EnemySpecialLayer,
	BombLayer,
	ExplosionLayer
};

class Bomberman : public AGame {
	typedef void (Bomberman::*Scene)(void);

   public:
	Bomberman(void);
	virtual ~Bomberman(void);
	virtual bool loadSceneByIndex(int sceneIdx);
	virtual size_t getWindowWidth();
	virtual size_t getWindowHeight();
	virtual bool isFullScreen();

   private:
	Save _save;
	std::map<std::string, Scene> _scenesMap;
	

	std::atomic_int loadState;

	void _initScenes(void);

	void _mainMenu(void);
	void _forest(void);
};