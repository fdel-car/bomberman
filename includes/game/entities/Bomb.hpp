#pragma once

#include "engine/Entity.hpp"

class Bomb : public Entity {
   public:
	Bomb(glm::vec3 position, float timer, size_t range, Entity *gameManager);
	virtual ~Bomb(void);

	virtual void update(void);

   private:
	float _timer;
	size_t _range;
};
