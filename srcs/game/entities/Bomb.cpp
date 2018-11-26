#include "game/entities/Bomb.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/scenes/SceneTools.hpp"

Bomb::Bomb(glm::vec3 position, float timer, size_t range, Entity *sceneManager)
	: Entity(
		  glm::vec3(position.x, position.y + 0.3f, position.z), glm::vec3(0),
		  new Collider(Collider::Rectangle, LayerTag::BombLayer, 0.49f, 0.49f),
		  "Bomb", "Bomb", "Bomb", sceneManager),
	  _timer(timer),
	  _range(range) {}

Bomb::~Bomb(void) {}

void Bomb::update(void) {
	float deltaTime = _gameEngine->getDeltaTime();

	_timer -= deltaTime;
	(void)_range;

	if (_timer <= 0.0f) {
		// SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);

		// if (cam == nullptr) {
		// 	cam->putExplosion(getPosition().x, getPosition().z, _range);
		// }
		_needToBeDestroyed = true;
	}
}
