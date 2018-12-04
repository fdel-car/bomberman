#pragma once

#include "engine/Entity.hpp"

enum PerkType { SpeedBoost = 0, BombRange, MaxBomb, Damage, KickBomb };

class Perk : public Entity {
   public:
	Perk(glm::vec3 position, Entity *sceneManager);
	virtual ~Perk(void);

	virtual void initEntity(GameEngine *gameEngine);
	virtual void update(void);
	virtual void onTriggerEnter(Entity *entity);

   private:
	static const float TIME_BETWEEN_ANIM;
	static const std::vector<std::tuple<PerkType, size_t>> POSSIBLE_PERKS;
	static const size_t TOTAL_PERK_PROBABILITY;

	PerkType _perkType;
	float _timer;
};
