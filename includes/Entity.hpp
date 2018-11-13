#pragma once

#include "Collider.hpp"
#include "header.hpp"

class GameEngine;

class Entity {
   public:
	Entity(std::vector<float> position, std::vector<float> rotation,
		   Collider *collider);
	virtual ~Entity(void);

	virtual void Update(void);
	GameEngine *getGameEngine(void) const;
	const std::vector<float> &getPosition(void) const;
	const std::vector<float> &getRotation(void) const;
	const Collider *getCollider(void) const;
	std::vector<float> &getTargetMovement(void);
	void moveFromPosition(std::vector<float> &newPos);
	void setGameEngine(GameEngine *gameEngine);

	// Model *model;
	// Texture *texture;
	// Animation *anim;
	// bool isTrigger;

   protected:
	Entity(void);
	Entity(Entity const &src);

	std::string _name;
	std::string _tag;
	GameEngine *_gameEngine;

	std::vector<float> _targetMovement;

   private:
	std::vector<float> _position;
	std::vector<float> _rotation;
	Collider *_collider;
};