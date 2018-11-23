#pragma once

#include "engine/Entity.hpp"

class Enemy : public Entity {
   public:
	Enemy(glm::vec3 position, glm::vec3 eulerAngles,
		  Entity *gameManager = nullptr);
	~Enemy(void);

	virtual void update(void);

   private:
	float _speed;
	float _cooldown = 0.0f;
	bool changeDir;

	Enemy(void);
	Enemy(Enemy const &src);

	Enemy &operator=(Enemy const &rhs);
};
