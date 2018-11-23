#pragma once

#include "engine/Entity.hpp"
#include "game/Save.hpp"

class Player : public Entity {
   public:
	Player(glm::vec3 position, glm::vec3 eulerAngles, Save &save,
		   Entity *gameManager = nullptr);
	~Player(void);

	virtual void update(void);

   private:
	Save &_save;
	float _speed;
	size_t _maxBombs;
	float _bombCooldown;
	size_t _bombRange;
	std::vector<float> _bombTimers;

	Player(void);
	Player(Player const &src);

	Player &operator=(Player const &rhs);
};