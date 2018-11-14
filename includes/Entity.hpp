#pragma once

#include "Collider.hpp"
#include "Shape.hpp"

class GameEngine;

class Entity {
   public:
	Entity(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
		   Shape *shape, bool isTmp);
	virtual ~Entity(void);

	virtual void Update(void);
	GameEngine *getGameEngine(void) const;
	const glm::vec3 &getPosition(void) const;
	const glm::mat4 &getModelMatrix(void) const;
	const Collider *getCollider(void) const;
	const Shape *getShape(void) const;
	bool getTmpState(void) const;
	void setGameEngine(GameEngine *gameEngine);

	void rotate(glm::vec3 axis, float angle);
	void rotateY(float angle);
	void translate(glm::vec3 translation);

	// Texture *texture;
	// Animation *anim;
	// bool isTrigger;

   protected:
	glm::vec3 _position;
	glm::mat4 _modelMatrix;

	Collider *_collider;
	Shape *_shape;

	std::string _name;
	std::string _tag;
	bool _isTmp;

	Entity(void);
	Entity(Entity const &src);

	GameEngine *_gameEngine;
};

typedef struct s_data {
	glm::vec3 pos;
	glm::vec3 eulerAngles;
	Collider *collider;
	std::string shapeName;
} t_data;

typedef std::vector<t_data> SceneData;