#pragma once

#include "game/entities/Damageable.hpp"

class Bomb : public Damageable {
   public:
	Bomb(glm::vec3 position, float timer, size_t range, Entity *gameManager);
	virtual ~Bomb(void);

	virtual void update(void);
	virtual void onDeath(void);
	virtual void onCollisionEnter(Entity *entity);

	void pushBomb(int xSign, int zSign, size_t pusherID);

   private:
	Bomb(Bomb const &src);
	Bomb &operator=(Bomb const &rhs);

	float _timer;
	size_t _range;
	size_t _pusherID;
	float _slideSpeed;
	float _xDirection;
	float _zDirection;
};
