#pragma once

#include "engine/AudioManager.hpp"
#include "engine/Camera.hpp"
#include "engine/Entity.hpp"
#include "engine/GUI/GUI.hpp"
#include "engine/GameRenderer.hpp"
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
	virtual void initAllAssets(void) = 0;
	virtual void initAllSounds(void) = 0;
	virtual size_t getWindowWidth() = 0;
	virtual size_t getWindowHeight() = 0;
	virtual bool isFullScreen() = 0;
	virtual int getSceneIndexByName(std::string sceneName) const;
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
	void setGameRenderer(GameRenderer *gameRenderer);
	void setAudioManager(AudioManager *audioManager);

	bool needResolutionChange;  // Public since lots of entities will need to
								// both get and set value

   protected:
	std::vector<std::string> _scenesNames;
	int _sceneIdx;
	std::vector<Entity *> _spawnableEntities;
	std::vector<Entity *> _entities;
	Camera *_camera;
	Light *_light;
	Skybox *_skybox;

	std::vector<Entity *> _loadingEntities;
	Camera *_loadingCamera;
	Light *_loadingLight;
	Skybox *_loadingSkybox;

	std::vector<std::tuple<float, std::string, std::string>> _neededFonts;
	std::map<std::string, std::string> _allAssets;  // keyword -> path
	std::set<std::string> _neededAssets;
	std::map<std::string, std::string> _allSounds;  // keyword -> path
	std::set<std::string> _neededSounds;
	std::vector<std::vector<bool>> _collisionTable;

	void setLayerCollision(int layer1, int layer2, bool doCollide);
	void loadAssets(void);
	void loadSounds(void);

   private:
	AGame(void);
	virtual void _initScenes(void) = 0;

	GameRenderer *_gameRenderer;
	AudioManager *_audioManager;
};
