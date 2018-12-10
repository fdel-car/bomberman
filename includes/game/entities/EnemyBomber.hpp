#pragma once

#include "engine/Entity.hpp"
#include "game/entities/AEnemy.hpp"

class EnemyBomber : public AEnemy {
   public:
	EnemyBomber(glm::vec3 position, glm::vec3 eulerAngles, std::string modelName,
			   Entity *gameManager = nullptr, Entity *toSpawn = nullptr);
	~EnemyBomber(void);

	virtual void update(void);

   private:
	EnemyBomber(void);
	EnemyBomber(EnemyBomber const &src);

	EnemyBomber &operator=(EnemyBomber const &rhs);
};
