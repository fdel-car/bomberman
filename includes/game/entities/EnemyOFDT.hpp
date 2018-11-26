#pragma once

#include "engine/Entity.hpp"
#include "game/entities/Enemy.hpp"

class EnemyOFDT : public Enemy {
   public:
	EnemyOFDT(glm::vec3 position, glm::vec3 eulerAngles,
		  Entity *gameManager = nullptr);
	~EnemyOFDT(void);

	virtual void update(void);

   private:
	float _speed;
	float _cooldown = 0.0f;
	bool changeDir;
	std::vector<float> _way;

	EnemyOFDT(void);
	EnemyOFDT(EnemyOFDT const &src);

	EnemyOFDT &operator=(EnemyOFDT const &rhs);
};
