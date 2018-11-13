#pragma once

#include "Collider.hpp"
#include "Model.hpp"
#include "header.hpp"

class GameEngine;

class Entity {
   public:
	Entity(glm::vec3 position, glm::vec3 rotation, Collider *collider,
		   Model *model, bool isTmp);
	virtual ~Entity(void);

	virtual void Update(void);
	GameEngine *getGameEngine(void) const;
	const glm::vec3 &getPosition(void) const;
	const glm::vec3 &getRotation(void) const;
	const Collider *getCollider(void) const;
	const Model *getModel(void) const;
	bool getTmpState(void) const;
	void setGameEngine(GameEngine *gameEngine);

	// Texture *texture;
	// Animation *anim;
	// bool isTrigger;

   protected:
	glm::vec3 _position;
	glm::vec3 _rotation;

	Collider *_collider;
	Model *_model;

	std::string _name;
	std::string _tag;
	bool _isTmp;

	Entity(void);
	Entity(Entity const &src);

	GameEngine *_gameEngine;
};

typedef struct s_data {
	glm::vec3 pos;
	glm::vec3 rot;
	Collider *collider;
	std::string modelName;
} t_data;

typedef std::vector<t_data> SceneData;