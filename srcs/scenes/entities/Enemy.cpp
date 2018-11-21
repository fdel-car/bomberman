#include "scenes/entities/Enemy.hpp"
#include "GameEngine.hpp"
#include "scenes/cams/ForestCam.hpp"

Enemy::Enemy(glm::vec3 position, glm::vec3 eulerAngles, Entity *gameManager)
	: Entity(position, eulerAngles,
			 new Collider(Collider::Circle, 0.48f, 0.48f), "Enemy",
			 gameManager) {
	_name = "Enemy";
	_tag = "Enemy";
	_speed = 1.0f;
}

Enemy::~Enemy(void) {}

void Enemy::update(void) {
	if (_cooldown <= 0.0f) {
		_cooldown = 0.5f;
		ForestCam *cam = reinterpret_cast<ForestCam *>(_sceneManager);
		if (cam != nullptr) {
			cam->printMapInfo();
		}

		float deltaTime = _gameEngine->getDeltaTime();
		int xSign = 1;
		int zSign = 0;

		// Normalize direction
		float xDirection = static_cast<float>(xSign);
		float zDirection = static_cast<float>(zSign);
		xSign = abs(xSign);
		zSign = abs(zSign);
		float totalMagnitude = xSign + zSign;
		xDirection *= sqrt(xSign / totalMagnitude);
		zDirection *= sqrt(zSign / totalMagnitude);

		_targetMovement.x = xDirection * _speed * deltaTime;
		_targetMovement.z = zDirection * _speed * deltaTime;
		changeDir = true;
	} else {
		_cooldown -= _gameEngine->getDeltaTime();

		if (_cooldown <= 0.25f && changeDir) {
			changeDir = false;
			float tmpVal = _targetMovement.z;
			_targetMovement.z = _targetMovement.x;
			_targetMovement.x = tmpVal;
		}
	}
	// float deltaTime = _gameEngine->getDeltaTime();

	// _targetMovement.x = xDirection * _speed * deltaTime;
	// _targetMovement.z = zDirection * _speed * deltaTime;
}
