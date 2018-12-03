#pragma once

#include "engine/AGame.hpp"
#include "engine/AudioManager.hpp"
#include "engine/Camera.hpp"
#include "engine/Collider.hpp"
#include "engine/Light.hpp"
#include "engine/Skybox.hpp"

#define EPSILON 0.01f

// For Threads
#include <atomic>
#include <thread>
typedef std::chrono::high_resolution_clock Clock;

struct KeyState {
	bool currFrame;
	bool prevFrame;
};

class GameRenderer;

class GameEngine {
   public:
	GameEngine(AGame *game);
	~GameEngine(void);

	void run();

	// Functions needed by Renderer
	GameRenderer const *getGameRenderer(void) const;
	Entity *getEntityById(size_t id);
	// Entity *getFirstEntityWithName(std::string entityName);
	// std::vector<Entity *> getEntitiesWithName(std::string entityName);
	// Entity *getFirstEntityWithLabel(std::string entityLabel);
	// std::vector<Entity *> getEntitiesWithLabel(std::string entityLabel);
	void buttonStateChanged(int keyID, bool isPressed);

	// Functions needed by entities
	bool isKeyPressed(int keyID);
	bool isKeyJustPressed(int keyID);
	float getDeltaTime();
	void addNewEntity(Entity *entity);
	void playMusic(std::string musicName);
	void playSound(std::string soundName);

   private:
	struct LineInfo {  // Equation of a line: z = mx + q
	   public:
		LineInfo(void);
		LineInfo(float startX, float startZ, float endX, float endZ);

		float m;
		float q;
		bool isVertical;  // if line is vertical then m = inf
		float startX;
		float startZ;
		float endX;
		float endZ;
	};

	struct RectanglePoints {
	   public:
		float top;
		float bot;
		float left;
		float right;
		RectanglePoints(Entity *entity, glm::vec3 movement = glm::vec3(0.0f));
	};
	static std::map<int, KeyState> _keyboardMap;

	GameEngine(void);
	GameEngine(GameEngine const &src);

	GameEngine &operator=(GameEngine const &rhs);

	bool _initScene(size_t newSceneIdx);
	void _unloadScene(void);
	void _loadScene(
		size_t newSceneIdx, std::atomic_int *_sceneState,
		bool *_checkLoadSceneIsGood);  // function to call for background Thread

	void _moveEntities(void);
	void _getPossibleCollisions(Entity *entity,
								std::vector<Entity *> &possibleCollisions,
								std::vector<Entity *> &possibleTriggers,
								std::vector<Entity *> &entitiesToTest);
	size_t _checkCollision(Entity *entity, glm::vec3 &futureMovement,
						   std::vector<Entity *> &collidedEntities);
	void _getMovementLines(Entity *entity, glm::vec3 &targetMovement,
						   LineInfo *lineA, LineInfo *lineB);
	bool _hasCollisionCourse(LineInfo &lineA, LineInfo &lineB, int layerTag,
							 Entity *entityB);
	bool _isLineLineCollision(LineInfo &lineA, LineInfo &lineB);
	bool _isLineCircleCollision(LineInfo &lineA, float &xSquareCoeff,
								float &xCoeff, float &zSquareCoeff,
								float &zCoeff, float &cCoeff);
	bool _doCollide(const Collider *colliderA, const glm::vec3 &posA,
					Entity *entityB) const;
	bool _collisionCircleRectangle(const Collider *circleCollider,
								   const glm::vec3 &circlePos,
								   const Collider *rectangleCollider,
								   const glm::vec3 &rectanglePos) const;
	bool _tryShortcut(Entity *entity, glm::vec3 &futureMovement,
					  glm::vec3 &shortcutMovement,
					  std::vector<Entity *> &collidedEntities);
	void _setSceneVariables(void);
	void _setLoadingSceneVariables(void);

	// Graphic libraries vars
	GameRenderer *_gameRenderer;
	Clock::time_point _frameTs;
	Clock::time_point _lastFrameTs;
	double _deltaTime;
	AudioManager *_audioManager;

	// Thread
	std::thread *_loadSceneThread;
	std::atomic_int _sceneState;
	Clock::time_point _timer;
	bool _checkLoadSceneIsGood;

	// Game model vars
	bool _running;
	bool _restartRequest;

	// Scene management vars
	int _sceneIdx;
	AGame *_game;
	Camera *_camera;
	Light *_light;
	Skybox *_skybox;
	std::vector<Entity *> _allEntities;

	Camera *_loadingCamera;
	Light *_loadingLight;
	Skybox *_loadingSkybox;
	std::vector<Entity *> _loadingAllEntities;

	std::vector<Entity *> _newEntities;
	std::map<size_t, std::vector<size_t>> _initialCollisionMap;
	std::vector<std::vector<bool>> const &_collisionTable;
};
