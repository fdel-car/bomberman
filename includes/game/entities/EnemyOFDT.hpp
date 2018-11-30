#pragma once

#include "engine/Entity.hpp"
#include "game/entities/AEnemy.hpp"

class EnemyOFDT : public AEnemy {
   public:
	EnemyOFDT(glm::vec3 position, glm::vec3 eulerAngles,
			  Entity *gameManager = nullptr);
	~EnemyOFDT(void);

	virtual void update(void);

   private:
	EnemyOFDT(void);
	EnemyOFDT(EnemyOFDT const &src);

	EnemyOFDT &operator=(EnemyOFDT const &rhs);
};
