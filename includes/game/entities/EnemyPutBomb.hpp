#pragma once

#include "engine/Entity.hpp"
#include "game/entities/AEnemy.hpp"

class EnemyPutBomb : public AEnemy {
   public:
	EnemyPutBomb(glm::vec3 position, glm::vec3 eulerAngles,
			  Entity *gameManager = nullptr);
	~EnemyPutBomb(void);

	virtual void update(void);

   private:
	EnemyPutBomb(void);
	EnemyPutBomb(EnemyPutBomb const &src);

	EnemyPutBomb &operator=(EnemyPutBomb const &rhs);
};
