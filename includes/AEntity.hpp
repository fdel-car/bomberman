#pragma once

#include "header.hpp"

class GameLogic;

class AEntity {
   public:
	AEntity(std::vector<float> position, std::vector<float> rotation);
	virtual ~AEntity(void) {}

	virtual void Update(void) = 0;

	void setGameLogic(GameLogic *gameLogic);

	std::vector<float> position;
	std::vector<float> rotation;

	std::string name;
	std::string tag;
	// Model *model;
	// Texture *texture;
	// Animation *anim;
	// Collider *collider;
	// bool isTrigger;

   protected:
	AEntity(void);

	GameLogic *_gameLogic;
};