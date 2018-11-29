#pragma once

#include "game/entities/Damageable.hpp"

class Bomb : public Damageable {
   public:
	Bomb(glm::vec3 position, float timer, size_t range, Entity *gameManager);
	virtual ~Bomb(void);

	virtual void update(void);
	virtual void onDeath(void);

   private:
	float _timer;
	size_t _range;
};
