#pragma once

#include "Collider.hpp"
#include "Shape.hpp"

class GameEngine;

class Entity {
   public:
	Entity(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
		   std::string shapeName);
	virtual ~Entity(void);

	GameEngine *getGameEngine(void) const;
	const glm::vec3 &getPosition(void) const;
	const glm::mat4 &getModelMatrix(void) const;
	const Collider *getCollider(void) const;
	const Shape *getShape(void) const;
	bool getTmpState(void) const;
	void initEntity(GameEngine *gameEngine);

	virtual void Update(void);

	void rotate(glm::vec3 axis, float angle);
	void rotateY(float angle);
	void translate(glm::vec3 translation);
	glm::vec3 &getTargetMovement(void);
	void moveFromPosition(glm::vec3 &newPos);

	// Texture *texture;
	// Animation *anim;
	// bool isTrigger;

   private:
	glm::vec3 _position;
	glm::mat4 _modelMatrix;

   protected:
	std::string _shapeName;
	std::string _name;
	std::string _tag;

	Shape *_shape;
	Collider *_collider;
	GameEngine *_gameEngine;
	bool _isTmp;
	glm::vec3 _targetMovement;

	Entity(void);
	Entity(Entity const &src);
};