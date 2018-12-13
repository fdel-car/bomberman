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
	virtual void onTakeDamage(
		std::vector<std::string> demagingSounds = std::vector<std::string>());
	virtual void onDeath(void);
	virtual void onCollisionEnter(Entity *entity);

	void gotSpeedBoost(float boost);
	void gotBombRangeBoost(size_t boost);
	void gotMaxBombBoost(size_t boost);
	void gotBombKickBoost(bool boost);

   private:
	Save &_save;
	float _speed;
	size_t _maxBombs;
	float _bombCooldown;
	size_t _bombRange;
	bool _bombKick;
	std::vector<float> _bombTimers;
	SceneTools *_cam;
	float _rotationAngle = 0.0f;

	Player(void);
	Player(Player const &src);

	Player &operator=(Player const &rhs);
};
