#pragma once

#include "Entity.hpp"

class Player : public Entity {
   public:
	Player(glm::vec3 position, glm::vec3 eulerAngles);
	~Player(void);

	virtual void update(void);

   private:
	float _speed;

	Player(void);
	Player(Player const &src);

	Player &operator=(Player const &rhs);
};