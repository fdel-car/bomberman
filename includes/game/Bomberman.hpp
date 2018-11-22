#pragma once

#include "engine/AGame.hpp"

struct Save {
   public:
	std::string upKey = "W";
	std::string downKey = "S";
};

// class json;

class Bomberman : public AGame {
	typedef void (Bomberman::*Scene)(void);

   public:
	Bomberman(void);
	virtual ~Bomberman(void);
	virtual bool loadSceneByIndex(int sceneIdx);

   private:
	std::map<std::string, Scene> _scenesMap;
	void _initScenes(void);

	void _mainMenu(void);
	void _forest(void);
};
