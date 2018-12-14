#pragma once

#include "engine/Entity.hpp"

class Explosion : public Entity {
   public:
	Explosion(glm::vec3 position, Entity *gameManager);
	virtual ~Explosion(void);

	virtual void update(void);
	virtual void onTriggerEnter(Entity *entity);

   private:
	Explosion(Explosion const &src);
	Explosion &operator=(Explosion const &rhs);

	float _timer;
	std::vector<std::string> _damagingSounds;
};
