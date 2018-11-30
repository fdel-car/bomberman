#pragma once

#include "engine/AGame.hpp"
#include "game/Save.hpp"

enum LayerTag {
	WallLayer = 0,
	BoxLayer,
	PlayerLayer,
	PlayerSpecialLayer,
	EnemyLayer,
	EnemyRunAwayLayer,
	EnemySpecialLayer,
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

	virtual bool loadSceneByIndex(int sceneIdx);
	virtual size_t getWindowWidth(void);
	virtual size_t getWindowHeight(void);
	virtual bool isFullScreen(void);
	virtual std::string getStartLevelName(void);

	Save &getSave(void);

   private:
	Save _save;
	const std::string _startLevelName;
	std::map<std::string, Scene> _scenesMap;

	void _initScenes(void);
	void _createMap(int width, int height);

	void _mainMenu(void);
	void _forest(void);
	void _volcano(void);
	void _desert(void);
};
