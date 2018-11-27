#pragma once

#include "game/entities/Damageable.hpp"

class Box : public Damageable {
   public:
	Box(glm::vec3 position, Entity *gameManager);
	virtual ~Box(void);

	virtual void update(void);
	virtual void onDeath(void);

   private:
	bool _onFire;
	float _timer;
};
