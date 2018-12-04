#include "game/entities/Explosion.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/entities/Damageable.hpp"
#include "game/scenes/SceneTools.hpp"

Explosion::Explosion(glm::vec3 position, Entity *sceneManager)
	: Entity(glm::vec3(position.x, position.y + 0.3f, position.z), glm::vec3(0),
			 new Collider(Collider::Rectangle, LayerTag::ExplosionLayer, 0.45f,
						  0.45f, true),
			 "Sphere", "Explosion", "Explosion", sceneManager),
	  _timer(1.0f),
	  _damagingSounds(std::vector<std::string>()) {
	scale(glm::vec3(0.8, 0.8, 0.8));
	setColor(glm::vec3(0.8, 0.6, 0.2));
	_damagingSounds.push_back("burn_player_1");
	_damagingSounds.push_back("burn_player_2");
}

Explosion::~Explosion(void) {}

void Explosion::update(void) {
	float deltaTime = _gameEngine->getDeltaTime();
	_timer -= deltaTime;

	if (_timer <= 0.0f) {
		_needToBeDestroyed = true;
	}
}

void Explosion::onTriggerEnter(Entity *entity) {
	Damageable *damageable = dynamic_cast<Damageable *>(entity);
	if (damageable != nullptr) {
		if (damageable->getName().compare("Player") == 0)
			damageable->takeDamage(_damagingSounds);
		else
			damageable->takeDamage();
	}
}
