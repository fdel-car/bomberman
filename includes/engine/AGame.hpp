#pragma once

#include "engine/Camera.hpp"
#include "engine/Entity.hpp"

class AGame {
   public:
	AGame(void);
	virtual ~AGame(void);

	virtual bool loadSceneByIndex(int sceneIdx) = 0;
	virtual size_t getWindowWidth() = 0;
	virtual size_t getWindowHeight() = 0;
	virtual bool isFullScreen() = 0;
	virtual int getSceneIndexByName(std::string sceneName) const;

	std::vector<std::tuple<float, std::string, std::string>> &getNeededFont();
	std::vector<Entity *> const getEntities() const;
	Camera *getCamera() const;

	void unload(void);

   protected:
	std::vector<std::string> _scenesNames;
	int _sceneIdx;
	std::vector<Entity *> _entities;
	Camera *_camera;
	std::vector<std::tuple<float, std::string, std::string>> _neededFonts;

   private:
	virtual void _initScenes(void) = 0;
};
