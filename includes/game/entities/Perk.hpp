#pragma once

#include "engine/Entity.hpp"

enum PerkType { SpeedBoost = 0, BombRange, MaxBomb, Damage, KickBomb };

class Perk : public Entity {
   public:
	static bool kickPerkDropped;

	Perk(glm::vec3 position, Entity *sceneManager);
	virtual ~Perk(void);

	virtual void onTriggerEnter(Entity *entity);

   private:
	static const std::vector<std::tuple<PerkType, size_t, std::string>>
		_possiblePerks;
	static const size_t _totalPerkProbs;
	static const std::vector<std::string> _damagingSounds;

	void _defineDroppedPerk(void);

	PerkType _perkType;
};
