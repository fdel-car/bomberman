#pragma once

#include "engine/Camera.hpp"
#include "engine/Engine.hpp"
#include "engine/Entity.hpp"
#include "engine/GUI/GUI.hpp"

class Tools : public Camera {
   public:
	Tools(size_t mapWidth, size_t mapHeight, glm::vec3 const &pos,
		  glm::vec3 const &eulerAngles);
	virtual ~Tools(void);

	virtual void tellPosition(Entity *entity);
	virtual void tellDestruction(Entity *entity);

	void printMapInfo(void);
	bool putBomb(float xCenter, float zCenter, float explosionTimer,
				 size_t range);
	void putExplosion(float xCenter, float zCenter, size_t range);

	std::map<size_t, std::vector<size_t>> const &getEntitiesInfos() const;
	std::vector<std::map<size_t, Entity *>> const &getEntitiesInSquares() const;
	size_t const &getMapWidth() const;
	size_t const &getMapHeight() const;

   protected:
	void _displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
						  int *startStrIdx, std::string name,
						  std::string imgName, std::string text, bool isImgLeft,
						  size_t maxCharPerLine, int nbrOfLine,
						  nk_flags textPosition, std::string fontText,
						  std::string fontTitle);
	bool _displayPauseMenu(GUI *graphicUI, int *_newSceneIdx, int restartIdx,
						   int leaveIdx);

	bool _btnHover(GUI *graphicUI, int rectWidth, int rectHeight, int xRectPos,
				   int yRectPos, int fontSize, std::string fontName,
				   int *extraSizePlay, int maxSize, bool *isPlayButtonHover,
				   std::string btnName, std::string btnImageHover = "",
				   std::string btnImage = "");

	bool _slowGUIAnimation;

	void _savePositions(Entity *entity);

	size_t _mapWidth;
	size_t _mapHeight;
	std::map<size_t, std::vector<size_t>> _entitiesInfos;
	std::vector<std::map<size_t, Entity *>> _entitiesInSquares;

   private:
	Tools(void);

	float _xOffset = static_cast<float>(_mapWidth) / 2;
	float _zOffset = static_cast<float>(_mapHeight) / 2;
};
