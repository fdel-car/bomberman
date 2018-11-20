#include "scenes/entities/Enemy.hpp"
#include "GameEngine.hpp"

Enemy::Enemy(glm::vec3 position, glm::vec3 eulerAngles, Entity *gameManager)
	: Entity(position, eulerAngles,
			 new Collider(Collider::Circle, 0.375f, 0.375f), "Enemy",
			 gameManager) {
	_name = "Enemy";
	_tag = "Enemy";
	_speed = 8.0f;
}

Enemy::~Enemy(void) {}

void Enemy::update(void) {
	// float deltaTime = _gameEngine->getDeltaTime();

	// _targetMovement.x = xDirection * _speed * deltaTime;
	// _targetMovement.z = zDirection * _speed * deltaTime;
}
