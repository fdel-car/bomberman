#pragma once

#include "header.hpp"

class GameEngine;

class AEntity {
   public:
	AEntity(GameEngine *_gameEngine, std::vector<float> position,
			std::vector<float> rotation);
	virtual ~AEntity(void) {}

	virtual void Update(void) = 0;

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

	GameEngine *_gameEngine;
};