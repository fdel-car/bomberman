#pragma once

#include "engine/AGame.hpp"
#include "game/Save.hpp"

//tmp includes
#include <chrono> 
#include <ctime>

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
	virtual void loadSceneByIndex(int sceneIdx, std::atomic_int *_sceneState, bool *_checkLoadSceneIsGood, GLFWwindow *_window);
	virtual void initLoadScene(int sceneIdx);
	virtual size_t getWindowWidth();
	virtual size_t getWindowHeight();
	virtual bool isFullScreen();

   private:
	Save _save;
	std::map<std::string, Scene> _scenesMap;
	GLFWwindow *_window; // to get current context in the Skybox thread or GL Function will segfault
	

	// std::atomic_int loadState;

	void _initScenes(void);

	void _mainMenu(void);
	void _forest(void);
	void _loadScene(void);
};