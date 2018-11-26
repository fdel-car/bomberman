#include "game/entities/Explosion.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/scenes/SceneTools.hpp"

Explosion::Explosion(glm::vec3 position, Entity *sceneManager)
	: Entity(glm::vec3(position.x, position.y + 0.3f, position.z), glm::vec3(0),
			 new Collider(Collider::Rectangle, LayerTag::ExplosionLayer, 0.4f,
						  0.4f, true),
			 "Box", "Explosion", "Explosion", sceneManager),
	  _timer(1.0f) {
	scale(glm::vec3(0.8, 0.8, 0.8));
}

Explosion::~Explosion(void) {}

void Explosion::update(void) {
	float deltaTime = _gameEngine->getDeltaTime();
	_timer -= deltaTime;

	if (_timer <= 0.0f) {
		_needToBeDestroyed = true;
	}
}
