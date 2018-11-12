#pragma once

#include "header.hpp"

class GameEngine;

class Entity {
   public:
	Entity(GameEngine *_gameEngine, std::vector<float> position,
		   std::vector<float> rotation);
	virtual ~Entity(void) {}

	virtual void Update(void);
	GameEngine *getGameEngine(void) const;
	const std::vector<float> &getPosition(void) const;
	const std::vector<float> &getRotation(void) const;
	// Model *model;
	// Texture *texture;
	// Animation *anim;
	// Collider *collider;
	// bool isTrigger;

   protected:
	std::vector<float> _position;
	std::vector<float> _rotation;

	std::string _name;
	std::string _tag;

	Entity(void);
	Entity(Entity const &src);

	GameEngine *_gameEngine;
};