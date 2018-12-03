#pragma once

#include "engine/Camera.hpp"
#include "engine/Entity.hpp"
#include "engine/Light.hpp"
#include "engine/Skybox.hpp"

#include <atomic>

class AGame {
   public:
	AGame(size_t enumSize);
	virtual ~AGame(void);

	virtual void loadSceneByIndex(int sceneIdx, std::atomic_int *_sceneState, bool *_checkLoadSceneIsGood) = 0;
	virtual void initLoadScene(void) = 0;
	virtual size_t getWindowWidth() = 0;
	virtual size_t getWindowHeight() = 0;
	virtual bool isFullScreen() = 0;
	virtual int getSceneIndexByName(std::string sceneName) const;
	virtual int getLoadingSceneIdx(void) const = 0;
	virtual int getFirstSceneIdx(void) const = 0;

	std::vector<std::tuple<float, std::string, std::string>> &getNeededFont();
	std::vector<Entity *> const getEntities() const;
	Camera *getCamera() const;
	Light *getLight() const;
	Skybox *getSkybox() const;
	std::vector<std::vector<bool>> const &getCollisionTable(void);
	const std::vector<std::tuple<std::string, std::string>> &getNeeededMusic()
		const;
	const std::vector<std::tuple<std::string, std::string>> &getNeeededSounds()
		const;

	

	void unload(void);

   protected:
	std::vector<std::string> _scenesNames;
	int _sceneIdx;
	std::vector<Entity *> _entities;
	Camera *_camera;
	Light *_light;

	std::vector<Entity *> _loadingEntities;
	Camera *_loadingCamera;
	Light *_loadingLight;
	Skybox *_loadingSkybox;

	Skybox *_skybox;
	std::vector<std::tuple<float, std::string, std::string>> _neededFonts;
	std::vector<std::tuple<std::string, std::string>> _neededMusic;
	std::vector<std::tuple<std::string, std::string>> _neededSounds;
	std::vector<std::vector<bool>> _collisionTable;

	void setLayerCollision(int layer1, int layer2, bool doCollide);

   private:
	AGame(void);
	virtual void _initScenes(void) = 0;
};
