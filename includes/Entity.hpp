#pragma once

#include "Collider.hpp"
#include "Model.hpp"

class GameEngine;

class Entity {
   public:
	Entity(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
		   std::string modelName);
	virtual ~Entity(void);

	GameEngine *getGameEngine(void) const;
	const glm::vec3 &getPosition(void) const;
	const glm::mat4 &getModelMatrix(void) const;
	const Collider *getCollider(void) const;
	const Model *getModel(void) const;
	bool getTmpState(void) const;
	glm::vec3 getEulerAngles(void) const;
	std::string const &getName(void) const;

	virtual void update(void);
	virtual void initEntity(GameEngine *gameEngine);

	void scale(glm::vec3 scale);
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

	Entity &operator=(Entity const &rhs);

   protected:
	std::string _modelName;
	std::string _name;
	std::string _tag;

	Model *_model;
	Collider *_collider;
	GameEngine *_gameEngine;
	bool _isTmp;

	glm::quat _rotation;
	glm::vec3 _targetMovement;

	Entity(void);
	Entity(Entity const &src);
};

std::ostream &operator<<(std::ostream &o, Entity const &entity);