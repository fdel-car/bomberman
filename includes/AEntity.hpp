#pragma once

#include "Collider.hpp"
#include "header.hpp"

class GameEngine;

class AEntity {
   public:
	AEntity(std::vector<float> position, std::vector<float> rotation,
			Collider collider);
	virtual ~AEntity(void) {}

	virtual void Update(void) = 0;

	void setGameEngine(GameEngine *gameEngine);

	std::vector<float> position;
	std::vector<float> rotation;

	std::string name;
	std::string tag;
	// Model *model;
	// Texture *texture;
	// Animation *anim;
	Collider collider;
	// bool isTrigger;

   protected:
	AEntity(void);

	GameEngine *_gameEngine;
};