#pragma once

#include "engine/Camera.hpp"
#include "engine/Engine.hpp"
#include "engine/Entity.hpp"
#include "engine/GUI/GUI.hpp"
#include "game/Bomberman.hpp"

struct Node {
   public:
	Node(void);
	Node(Node *prev, size_t dist, size_t x, size_t z, size_t id);
	~Node(void);

	void newNode(Node *prev, size_t dist, size_t x, size_t z, size_t id);
	void updateNode(Node *old, size_t dist, bool saveInPrevious);
	void clear(void);
	// previous nodes by distance from the target
	std::map<size_t, std::vector<Node *>> prevNodesByDist;
	// next nodes by distance from the target
	std::map<size_t, std::vector<Node *>> runAwayNodesByDist;
	// Entities with hitbox on this node
	std::vector<Entity *> entitiesOnMe;

	size_t dist;
	size_t runAwayDist;
	size_t x;
	size_t z;
	size_t id;
	bool isAnEntity;
};

struct Dialogue {
	int searchWord;
	int lastWord;
	int startStrIdx;
	std::string name;
	std::string imgName;
	std::string text;
	bool isImgLeft;
	size_t maxCharPerLine;
	int nbrOfLine;
	nk_flags textPosition;
	std::string fontText;
	std::string fontTitle;
};

struct WorldLocation {
	glm::vec3 pos = glm::vec3(0);
	glm::vec3 rot = glm::vec3(0);
	WorldLocation(glm::vec3 pos, glm::vec3 rot);
};

class SceneTools : public Camera {
   public:
	SceneTools(size_t mapWidth, size_t mapHeight,
			   WorldLocation &dialogueLocation, WorldLocation &gameplayLocation,
			   float transitionTime, Bomberman *bomberman,
			   std::string ownLvlName = "", std::string nextLvlName = "",
			   float timer = 181.0f);
	SceneTools(size_t mapWidth, size_t mapHeight, WorldLocation &startLocation,
			   Bomberman *bomberman, std::string ownLvlName = "",
			   std::string nextLvlName = "", float timer = 181.0f);
	virtual ~SceneTools(void);

	virtual void initEntity(GameEngine *gameEngine);
	virtual void drawGUI(GUI *graphicUI);
	virtual bool isPause(void) const;
	virtual void updateDebugMode(void);
	virtual void tellPosition(Entity *entity);
	virtual void tellDestruction(Entity *entity);
	virtual void configAI(void);

	void printMapInfo(void);
	bool canPutBomb(float xCenter, float zCenter);
	bool putBomb(float xCenter, float zCenter, float explosionTimer,
				 size_t range);
	void putExplosion(float xCenter, float zCenter, size_t range);
	void tellPlayerHp(size_t hp);
	void tellLevelSuccess();

	size_t const &getPlayerPos() const;
	size_t const &getRunAwayPos() const;
	std::map<size_t, std::vector<size_t>> const &getEntitiesInfos() const;
	std::vector<std::map<size_t, Entity *>> const &getEntitiesInSquares() const;
	size_t const &getMapWidth() const;
	size_t const &getMapHeight() const;
	std::map<size_t, Node *> const &getGraphe() const;
	bool const &getRefreshAI() const;
	void gotSpeedBoost(float speed);
	void gotRangeBoost(int range);
	void gotMaxBombBoost(int maxBomb);
	void gotBombKickBoost(bool kick);
	void setPerksValues(float speed, int maxBomb, int range, bool kick);



   protected:
	void _initSoundsForGameplay(void);

	// General tool
	bool _btnHover(GUI *graphicUI, int rectWidth, int rectHeight, int xRectPos,
				   int yRectPos, int fontSize, std::string fontName,
				   int *extraSizePlay, int maxSize, bool *isPlayButtonHover,
				   std::string btnName, std::string btnImageHover = "",
				   std::string btnImage = "");

	// Dialogue logic
	bool _displayMultipleDialogue(GUI *graphicUI);
	void _displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
						  int *startStrIdx, std::string name,
						  std::string imgName, std::string text, bool isImgLeft,
						  size_t maxCharPerLine, int nbrOfLine,
						  nk_flags textPosition, std::string fontText,
						  std::string fontTitle);
	void _buildNewDialogue(int searchWord, int lastWord, int startStrIdx,
						   std::string name, std::string imgName,
						   std::string text, bool isImgLeft,
						   size_t maxCharPerLine, int nbrOfLine,
						   nk_flags textPosition, std::string fontText,
						   std::string fontTitle);

	// Transition tool
	void _doTransition(void);

	// Gameplay tool
	void _savePositions(Entity *entity);

	// Display during gameplay
	void _gameplayDisplay(GUI *graphicUI);
	bool _displayPauseMenu(GUI *graphicUI);
	void _displayPlayerHP(GUI *graphicUI, size_t hp);
	void _displayVictoryScreen(GUI *graphicUI);
	void _displayDeathScreen(GUI *graphicUI);
	void _displayTimer(GUI *graphicUI, bool isPause);

	// Build graph
	void _startBuildingGrapheForPathFinding(void);
	void _searchWay(bool searchBestWay, Node *originNode, size_t playerPos);
	void _buildNewNode(size_t dist, size_t x, size_t z, size_t pos, Node *node,
					   std::list<Node *> *nodesByDepth, bool saveInPrevious);
	void _clearGraphe(void);
	void _describeNode(Node *n);



	Bomberman *_bomberman;
	Save &_save;
	bool _slowGUIAnimation;
	size_t _playerPos;
	size_t _runAwayPos;
	size_t _playerMaxHp;
	size_t _playerHp;
	bool _showPlayerHp;
	bool _showVictoryScreen;
	bool _showDeathScreen;
	size_t _mapWidth;
	size_t _mapHeight;
	std::map<size_t, std::vector<size_t>> _entitiesInfos;
	std::vector<std::map<size_t, Entity *>> _entitiesInSquares;
	std::map<size_t, Node *> _graphe;
	std::vector<std::string>
		_staticDecor;  // Decor who can't be destroy (like arena walls)
	std::vector<std::string>
		_tmpDecor;  // Decor who can be destroy (like bombes or brick walls)
	std::vector<std::string> _tmpDecorForRunAway;
	bool _refreshAI;
	bool _firstPlayerPos;
	glm::vec3 _positionPrevDebug;
	glm::vec3 _eulerAnglesPrevDebug;
	glm::vec3 _distanceFromPlayer;
	std::string _startMusic;
	float _speedBoost = 0.0f;
	int _rangeBoost = 0;
	int _maxBombBoost = 0;
	bool _bombKickBoost = false;

   private:
	SceneTools(void);

	void _putExplosionsInDirection(size_t xCoord, size_t zCoord, int xChange,
								   int zChange, size_t range,
								   bool &hasDestroyedBox);

	float _xOffset = static_cast<float>(_mapWidth) / 2;
	float _zOffset = static_cast<float>(_mapHeight) / 2;
	std::vector<Dialogue> _dialogues = std::vector<Dialogue>();
	WorldLocation _currentLocation;
	WorldLocation _dialogueLocation;
	WorldLocation _gameplayLocation;
	glm::vec3 _pastRotation;
	float _transitionTime;
	float _transitionElapsedTime = 0.0f;
	std::vector<Node *> nodePool;
	std::string _ownLvlName;
	std::string _startLvlName;
	std::string _nextLvlName;

	float _timer;
	bool _pauseMenu;

	std::vector<std::string> _explosionSounds;
	std::vector<std::string> _explosionWithBoxSounds;
};
