#pragma once

#include "game/entities/Damageable.hpp"

class Box : public Damageable {
   public:
	Box(glm::vec3 position, Entity *gameManager, std::string modelName,
		int perkProb = 33, Entity *toSpawn = nullptr);
	virtual ~Box(void);

	virtual void update(void);
	virtual void onDeath(void);

   private:
	bool _onFire;
	bool _hasSpawned;
	float _timer;
	int _perkProb;
	Entity *_toSpawn = nullptr;
};
