#pragma once

#include "engine/Entity.hpp"
#include "game/entities/AEnemy.hpp"

class EnemyMelee : public AEnemy {
   public:
	EnemyMelee(glm::vec3 position, glm::vec3 eulerAngles, std::string modelName,
			   Entity *gameManager = nullptr, Entity *toSpawn = nullptr);
	~EnemyMelee(void);

	virtual void update(void);

   private:
	EnemyMelee(void);
	EnemyMelee(EnemyMelee const &src);

	EnemyMelee &operator=(EnemyMelee const &rhs);
};
