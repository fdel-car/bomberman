#include "game/entities/Box.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/scenes/SceneTools.hpp"

Box::Box(glm::vec3 position, Entity *sceneManager)
	: Entity(glm::vec3(position.x, position.y + 0.4f, position.z), glm::vec3(0),
			 new Collider(Collider::Rectangle, LayerTag::BoxLayer, 0.45f, 0.45f,
						  false),
			 "Box", "Box", "Box", sceneManager),
	  _onFire(false),
	  _timer(1.0f) {
	scale(glm::vec3(1, 0.8, 1));
}

Box::~Box(void) {}

void Box::update(void) {
	if (!_onFire) return;

	float deltaTime = _gameEngine->getDeltaTime();
	_timer -= deltaTime;

	if (_timer <= 0.0f) {
		_needToBeDestroyed = true;
	}
}
