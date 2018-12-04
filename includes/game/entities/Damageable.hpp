#pragma once

#include "engine/Entity.hpp"

class Damageable : public Entity {
   public:
	Damageable(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
			   std::string modelName, std::string name, std::string tag,
			   size_t hp, int baseLayer, int damagedLayer,
			   float _damagedMaxTime, Entity *sceneManager = nullptr);
	~Damageable(void);

	virtual void update(void);
	virtual void takeDamage(
		std::vector<std::string> demagingSounds = std::vector<std::string>());
	virtual void onTakeDamage(
		std::vector<std::string> demagingSounds = std::vector<std::string>());
	virtual void onDeath(void);

   protected:
	bool _alive;
	size_t _hp;
	int _baseLayer;
	int _damagedLayer;
	float _damagedMaxTime;
	float _timeDamaged;

   private:
	Damageable(void);
	Damageable(Damageable const &src);
	Damageable &operator=(Damageable const &rhs);
};
