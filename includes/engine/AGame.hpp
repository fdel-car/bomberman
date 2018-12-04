#pragma once

#include "engine/AudioManager.hpp"
#include "engine/Camera.hpp"
#include "engine/Entity.hpp"
#include "engine/Light.hpp"
#include "engine/Skybox.hpp"

#include <atomic>

class AGame {
   public:
	AGame(size_t enumSize);
	virtual ~AGame(void);

	virtual void loadSceneByIndex(int sceneIdx, std::atomic_int *_sceneState,
								  bool *_checkLoadSceneIsGood) = 0;
	virtual void initLoadScene(void) = 0;
	virtual size_t getWindowWidth() = 0;
	virtual size_t getWindowHeight() = 0;
	virtual bool isFullScreen() = 0;
	virtual int getSceneIndexByName(std::string sceneName) const;
	virtual int getLoadingSceneIdx(void) const = 0;
	virtual int getFirstSceneIdx(void) const = 0;
	virtual int getStartingMusicVolume(void) const;
	virtual int getStartingSoundsVolume(void) const;

	std::vector<std::tuple<float, std::string, std::string>> &getNeededFont();
	std::vector<Entity *> const getEntities() const;
	Camera *getCamera() const;
	Light *getLight() const;
	Skybox *getSkybox() const;

	std::vector<Entity *> const getLoadingEntities() const;
	Camera *getLoadingCamera() const;
	Light *getLoadingLight() const;
	Skybox *getLoadingSkybox() const;
	std::vector<std::vector<bool>> const &getCollisionTable(void);

	void unload(void);
	void setAudioManager(AudioManager *audioManager);

   protected:
	std::vector<std::string> _scenesNames;
	int _sceneIdx;
	std::vector<Entity *> _entities;
	Camera *_camera;
	Light *_light;
	Skybox *_skybox;

	std::vector<Entity *> _loadingEntities;
	Camera *_loadingCamera;
	Light *_loadingLight;
	Skybox *_loadingSkybox;

	std::vector<std::tuple<float, std::string, std::string>> _neededFonts;
	std::vector<std::vector<bool>> _collisionTable;

	void setLayerCollision(int layer1, int layer2, bool doCollide);
	void loadSounds(void);

   private:
	AGame(void);
	virtual void _initScenes(void) = 0;

	AudioManager *_audioManager;
};
