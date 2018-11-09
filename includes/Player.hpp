#pragma once

#include "AEntity.hpp"

class Player : public AEntity
{
  public:
	Player(void);
	~Player(void);
	Player(Player const &src);
	Player &operator=(Player const &rhs);

	virtual void Update(void);
};