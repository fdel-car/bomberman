#pragma once

#include "engine/Entity.hpp"

class Explosion : public Entity {
   public:
	Explosion(glm::vec3 position, Entity *gameManager);
	virtual ~Explosion(void);

	virtual void update(void);
	virtual void onTriggerEnter(Entity *entity);

   private:
	float _timer;
};
