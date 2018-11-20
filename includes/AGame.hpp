#pragma once

#include "Camera.hpp"
#include "Entity.hpp"

class AGame {
   public:
	AGame(void);
	virtual ~AGame(void);

	virtual bool loadSceneByIndex(int sceneIdx) = 0;
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
