#pragma once

#include "game/entities/Damageable.hpp"
#include "game/scenes/SceneTools.hpp"

class AEnemy : public Damageable {
   public:
	AEnemy(glm::vec3 position, glm::vec3 eulerAngles,
		   Entity *gameManager = nullptr);
	~AEnemy(void);

	virtual void update(void) = 0;

   protected:
	void findBestWay(SceneTools *cam, bool runAway = false);
	void walk(SceneTools *cam);

	float _speed;
	bool changeDir;
	std::vector<float> _way;

   private:
	AEnemy(void);
	AEnemy(AEnemy const &src);

	AEnemy &operator=(AEnemy const &rhs);
};