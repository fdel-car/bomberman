#pragma once

#include "AEntity.hpp"

class Player : public AEntity
{
  public:
	Player(GameLogic *gameLogic, std::vector<float> position, std::vector<float> rotation);
	~Player(void);
	Player(Player const &src);
	Player &operator=(Player const &rhs);

	virtual void Update(void);

  private:
	Player(void);

	float _speed;
};