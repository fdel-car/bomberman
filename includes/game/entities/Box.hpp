#pragma once

#include "engine/Entity.hpp"

class Box : public Entity {
   public:
	Box(glm::vec3 position, Entity *gameManager);
	virtual ~Box(void);

	virtual void update(void);

   private:
	bool _onFire;
	float _timer;
};
