#pragma once

#include "game/Bomberman.hpp"
#include "game/entities/Damageable.hpp"
#include "game/scenes/SceneTools.hpp"

class AEnemy : public Damageable {
   public:
	AEnemy(glm::vec3 position, glm::vec3 eulerAngles, std::string name,
		   LayerTag tag, bool doMeleeDmg, Entity *gameManager,
		   std::string modelName, Entity *toSpawn = nullptr);
	~AEnemy(void);

	virtual void update(void) = 0;
	virtual void onCollisionEnter(Entity *entity);
	virtual void onDeath(void);

   protected:
	void findBestWay(SceneTools *cam, size_t distFromPlayer = 0,
					 bool runAway = false, bool putBomb = false);
	void randomMove(SceneTools *cam, float timer);
	void walk(SceneTools *cam);

	float _speed;
	float _bombCooldown;
	float _resetMoveCoolDown;
	bool _doMeleeDmg;
	bool changeDir;
	std::vector<float> _way;

   private:
	bool _hasSpawned;
	Entity *_toSpawn;
	std::vector<std::string> _damagingSounds;

	void _runIn(SceneTools *cam, size_t distFromPlayer, bool putBomb);
	void _runAway(SceneTools *cam, size_t distFromPlayer, bool putBomb);
	AEnemy(void);
	AEnemy(AEnemy const &src);

	AEnemy &operator=(AEnemy const &rhs);
};
