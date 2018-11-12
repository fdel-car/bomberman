#pragma once

#include "Collider.hpp"
#include "header.hpp"

class GameEngine;

class Entity {
   public:
	Entity(std::vector<float> position, std::vector<float> rotation,
		   Collider collider);
	virtual ~Entity(void) {}

	virtual void Update(void);
	GameEngine *getGameEngine(void) const;
	const std::vector<float> &getPosition(void) const;
	const std::vector<float> &getRotation(void) const;
	const Collider &getCollider(void) const;
	void setGameEngine(GameEngine *gameEngine);

	// Model *model;
	// Texture *texture;
	// Animation *anim;
	// bool isTrigger;

   protected:
	std::vector<float> _position;
	std::vector<float> _rotation;

	Collider _collider;

	std::string _name;
	std::string _tag;

	Entity(void);
	Entity(Entity const &src);

	GameEngine *_gameEngine;
};