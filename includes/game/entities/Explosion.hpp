#pragma once

#include "engine/Entity.hpp"

class Explosion : public Entity {
   public:
	Explosion(glm::vec3 position, Entity *gameManager);
	virtual ~Explosion(void);

	virtual void update(void);

   private:
	float _timer;
};
