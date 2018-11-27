#pragma once

#include "game/Save.hpp"
#include "game/entities/Damageable.hpp"
#include "game/scenes/SceneTools.hpp"

class Player : public Damageable {
   public:
	Player(glm::vec3 position, glm::vec3 eulerAngles, Save &save,
		   Entity *gameManager = nullptr);
	~Player(void);

	virtual void update(void);
	virtual void onTakeDamage(void);
	virtual void onDeath(void);

   private:
	Save &_save;
	float _speed;
	size_t _maxBombs;
	float _bombCooldown;
	size_t _bombRange;
	std::vector<float> _bombTimers;
	SceneTools *_cam;

	Player(void);
	Player(Player const &src);

	Player &operator=(Player const &rhs);
};