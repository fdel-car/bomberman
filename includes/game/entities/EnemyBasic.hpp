#pragma once

#include "engine/Entity.hpp"
#include "game/entities/AEnemy.hpp"

class EnemyBasic : public AEnemy {
   public:
	EnemyBasic(glm::vec3 position, glm::vec3 eulerAngles, std::string modelName,
			  Entity *gameManager = nullptr);
	~EnemyBasic(void);

	virtual void update(void);

   private:
	EnemyBasic(void);
	EnemyBasic(EnemyBasic const &src);

	EnemyBasic &operator=(EnemyBasic const &rhs);
};
