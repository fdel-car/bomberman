#pragma once

#include "engine/Camera.hpp"
#include "engine/Entity.hpp"

class AGame {
   public:
	AGame(size_t enumSize);
	virtual ~AGame(void);

	virtual bool loadSceneByIndex(int sceneIdx) = 0;
	virtual size_t getWindowWidth() = 0;
	virtual size_t getWindowHeight() = 0;
	virtual bool isFullScreen() = 0;
	virtual int getSceneIndexByName(std::string sceneName) const;

	std::vector<std::tuple<float, std::string, std::string>> &getNeededFont();
	std::vector<Entity *> const getEntities() const;
	Camera *getCamera() const;
	std::vector<std::vector<bool>> const &getCollisionTable(void);

	void unload(void);

   protected:
	std::vector<std::string> _scenesNames;
	int _sceneIdx;
	std::vector<Entity *> _entities;
	Camera *_camera;
	std::vector<std::tuple<float, std::string, std::string>> _neededFonts;
	std::vector<std::vector<bool>> _collisionTable;

	void setLayerCollision(int layer1, int layer2, bool doCollide);

   private:
	AGame(void);
	virtual void _initScenes(void) = 0;
};