#include "bomberman/entities/Enemy.hpp"
#include "bomberman/cams/ForestCam.hpp"
#include "engine/GameEngine.hpp"

Enemy::Enemy(glm::vec3 position, glm::vec3 eulerAngles, Entity *gameManager)
	: Entity(position, eulerAngles, new Collider(Collider::Circle, 0.4f, 0.4f),
			 "Enemy", gameManager) {
	_name = "Enemy";
	_tag = "Enemy";
	_speed = 4.0f;
	scale(glm::vec3(0.8, 0.8, 0.8));
}

Enemy::~Enemy(void) {}

void Enemy::update(void) {
	// if (_cooldown <= 0.0f) {
	// 	_cooldown = 0.5f;
	Tools *cam = reinterpret_cast<Tools *>(_sceneManager);

	if (cam == nullptr) {
		std::cerr << "Update as fail" << std::endl;
		return;
	}
	size_t mapWidth = cam->getMapWidth();
	size_t mapHeight = cam->getMapHeight();
	size_t posIdx = cam->getEntitiesInfos().at(_id)[0];

	float x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
	float z = cam->getEntitiesInSquares()[posIdx].at(_id)->getPosition().z +
			  (static_cast<float>(mapHeight) / 2) + _collider->height;
	if (_cooldown <= 0.0f) {
		_cooldown = 0.5f;
		cam->printMapInfo();
		// std::cout << "X : " << x << " Z : " << z << std::endl;
		// std::cout << "X : " << static_cast<int>(x)
		// 		  << " Z : " << static_cast<int>(z) << std::endl;
		// std::cout << "Check: " << x << std::endl;
	}
	// 	float deltaTime = _gameEngine->getDeltaTime();
	if (x - 8.5f <= 0.05)
		_targetMovement *= 0;
	else {
		int xSign = -1;
		int zSign = 0;
		//
		// 	// Normalize direction
		float xDirection = static_cast<float>(xSign);
		float zDirection = static_cast<float>(zSign);
		// 	xSign = abs(xSign);
		// 	zSign = abs(zSign);
		float totalMagnitude = xSign + zSign;
		xDirection *= sqrt(xSign / totalMagnitude);
		zDirection *= sqrt(zSign / totalMagnitude);
		//
		// 	_targetMovement.x = xDirection * _speed * deltaTime;
		// 	_targetMovement.z = zDirection * _speed * deltaTime;
		// 	changeDir = true;
		// } else {
		_cooldown -= _gameEngine->getDeltaTime();
		//
		// 	if (_cooldown <= 0.25f && changeDir) {
		// 		changeDir = false;
		// 		float tmpVal = _targetMovement.z;
		// 		_targetMovement.z = _targetMovement.x;
		// 		_targetMovement.x = tmpVal;
		// 	}
		// }

		float deltaTime = _gameEngine->getDeltaTime();
		//
		_targetMovement.x = xDirection * _speed * deltaTime;
		_targetMovement.z = zDirection * _speed * deltaTime;
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
