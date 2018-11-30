#pragma once

#include "engine/Camera.hpp"
#include "engine/Engine.hpp"
#include "engine/Entity.hpp"
#include "engine/GUI/GUI.hpp"

struct Node {
   public:
	Node();
	Node(Node *prev, size_t dist, size_t x, size_t z, size_t id);
	~Node();

	void updateNode(Node *old, size_t dist, bool saveInPrevious);
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
	bool isFatal;
	bool isAnEntity;
};

class SceneTools : public Camera {
   public:
	SceneTools(size_t mapWidth, size_t mapHeight, glm::vec3 const &pos,
			   glm::vec3 const &eulerAngles);
	virtual ~SceneTools(void);

	virtual bool isPause(void) const;

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

   protected:
	void _displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
						  int *startStrIdx, std::string name,
						  std::string imgName, std::string text, bool isImgLeft,
						  size_t maxCharPerLine, int nbrOfLine,
						  nk_flags textPosition, std::string fontText,
						  std::string fontTitle);
	bool _displayPauseMenu(GUI *graphicUI, int *_newSceneIdx, int restartIdx,
						   int leaveIdx);
	void _displayPlayerHP(GUI *graphicUI, size_t hp);
	void _displayVictoryScreen(GUI *graphicUI, int *_newSceneIdx, int nextIdx,
							   int restartIdx, int leaveIdx);
	void _displayDeathScreen(GUI *graphicUI, int *_newSceneIdx, int restartIdx,
							 int leaveIdx);
	void _displayTimer(GUI *graphicUI, float *currentTime, bool isPause);
	bool _btnHover(GUI *graphicUI, int rectWidth, int rectHeight, int xRectPos,
				   int yRectPos, int fontSize, std::string fontName,
				   int *extraSizePlay, int maxSize, bool *isPlayButtonHover,
				   std::string btnName, std::string btnImageHover = "",
				   std::string btnImage = "");
	void _savePositions(Entity *entity);

	// Build graphe
	void _startBuildingGrapheForPathFinding(void);
	void _searchWay(bool searchBestWay, Node *originNode, size_t playerPos);
	void _buildNewNode(size_t dist, size_t x, size_t z, size_t pos, Node *node,
					   std::list<Node *> *nodesByDepth, bool saveInPrevious);
	void _clearGraphe(void);
	void _describeNode(Node *n);

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
	std::vector<std::string>
			_tmpDecorForRunAway;
	bool _refreshAI;

   private:
	SceneTools(void);

	void _putExplosionsInDirection(size_t xCoord, size_t zCoord, int xChange,
								   int zChange, size_t range);

	float _xOffset = static_cast<float>(_mapWidth) / 2;
	float _zOffset = static_cast<float>(_mapHeight) / 2;
};
