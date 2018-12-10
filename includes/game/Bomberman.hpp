#pragma once

#include "engine/AGame.hpp"
#include "game/Save.hpp"

// tmp includes
#include <chrono>
#include <ctime>

enum LayerTag {
	WallLayer = 0,
	BoxLayer,
	PlayerLayer,
	PlayerSpecialLayer,
	EnemyLayer,
	EnemyRunAwayLayer,
	EnemyBomberLayer,
	EnemyMeleeLayer,
	EnemySpecialLayer,
	EnemyBasicLayer,
	BombLayer,
	ExplosionLayer,
	PerkLayer,
	PortalLayer
};

class Bomberman : public AGame {
	typedef void (Bomberman::*Scene)(void);

   public:
	Bomberman(void);
	virtual ~Bomberman(void);
	virtual void loadSceneByIndex(int sceneIdx, std::atomic_int *_sceneState,
								  bool *_checkLoadSceneIsGood);
	virtual void initLoadScene(void);
	virtual void initAllAssets(void);
	virtual void initAllSounds(void);
	virtual size_t getWindowWidth();
	virtual size_t getWindowHeight();
	virtual bool isFullScreen();
	virtual std::string getStartLevelName(void);
	virtual int getFirstSceneIdx(void) const;
	virtual int getStartingMusicVolume(void) const;
	virtual int getStartingSoundsVolume(void) const;

	Save &getSave(void);

   private:
	Save _save;
	const std::string _startLevelName;
	std::map<std::string, Scene> _scenesMap;
	// GLFWwindow *_window; // to get current context in the Skybox thread or GL
	// Function will segfault

	// std::atomic_int loadState;

	void _initScenes(void);
	void _createMap(int width, int height,
					std::vector<std::tuple<int, int>> &protectedCase,
					size_t spwanRate, size_t monsterRate,
					std::vector<std::string> border,
					std::vector<std::string> undestrutibleBlock,
					std::vector<std::string> destrutibleBlock,
					std::vector<std::string> enemies);

	void _mainMenu(void);
	void _forest(void);
	void _pokemon(void);
	void _loadScene(void);
	void _mario(void);
	void _space(void);
	void _credits(void);

	void _initAssetsForGameplay(void);
	void _initSoundsForGameplay(void);
};
