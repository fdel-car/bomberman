#pragma once

#include "engine/AGame.hpp"

struct Save {
   public:
	std::string upKey = "W";
	std::string leftKey = "A";
	std::string downKey = "S";
	std::string rightKey = "D";
	std::string bombKey = "SPACE";
	size_t level = 0;

	void resetSettings(void);
	void resetLevel(void);
	void resetAll(void);
};

class BombermanGame : public AGame {
	typedef void (BombermanGame::*Scene)(void);

   public:
	static Save save;
	static void initNewSave(void);
	static void doSave(void);

	BombermanGame(void);
	virtual ~BombermanGame(void);
	virtual bool loadSceneByIndex(int sceneIdx);

   private:
	static const std::string gameDir;
	static const std::string SAVE_FILE;

	static void getSave(void);

	std::map<std::string, Scene> _scenesMap;

	void _initScenes(void);

	void _mainMenu(void);
	void _forest(void);
};