#pragma once

#include "Entity.hpp"

class Enemy : public Entity {
   public:
	Enemy(glm::vec3 position, glm::vec3 eulerAngles);
	~Enemy(void);

	virtual void update(void);

   private:
	float _speed;

	Enemy(void);
	Enemy(Enemy const &src);

	Enemy &operator=(Enemy const &rhs);
};