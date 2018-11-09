#pragma once

#include "header.hpp"

class AEntity
{
  public:
	AEntity(void);
	virtual ~AEntity(void) {}

	virtual void Update(void) = 0;

	std::vector<float> position;
	std::vector<float> rotation;

	bool isTrigger;
	// Model *model;
	// Texture *texture;
	// Animation *anim;
	// Collider *collider;
};