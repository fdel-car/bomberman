#pragma once

#include "Entity.hpp"

class Player : public Entity {
   public:
	Player(std::vector<float> position, std::vector<float> rotation);
	~Player(void);
	Player(Player const &src);
	Player &operator=(Player const &rhs);

	virtual void Update(void);

   private:
	Player(void);

	float _speed;
};