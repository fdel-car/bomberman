#pragma once

#include "engine/Entity.hpp"
#include "game/entities/AEnemy.hpp"

class EnemyRunAway : public AEnemy {
   public:
	EnemyRunAway(glm::vec3 position, glm::vec3 eulerAngles, std::string modelName,
				 Entity *gameManager = nullptr, Entity *toSpawn = nullptr);
	~EnemyRunAway(void);

	virtual void update(void);

   private:
	EnemyRunAway(void);
	EnemyRunAway(EnemyRunAway const &src);

	EnemyRunAway &operator=(EnemyRunAway const &rhs);
};
