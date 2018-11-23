#include "game/entities/Enemy.hpp"
#include "engine/GameEngine.hpp"
#include "game/cams/ForestCam.hpp"

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
	Tools *cam = reinterpret_cast<Tools *>(_sceneManager);
	static std::vector<float> tmp;
	size_t mapWidth = cam->getMapWidth();
	size_t mapHeight = cam->getMapHeight();
	if (tmp.size() == 0) {

		// tmp.push_back((mapHeight) * 7.0f + 15.0f);
		// tmp.push_back((mapHeight) * 7.0f + 1.0f);
		tmp.push_back((mapHeight) * 15.0f + 7.0f);
		tmp.push_back((mapHeight) * 1.0f + 7.0f);
		tmp.push_back((mapHeight) * 1.0f + 1.0f);
	}



	if (cam == nullptr) {
		std::cerr << "Update as fail" << std::endl;
		return;
	}

	float targetX = (static_cast<int>(tmp[0]) % (mapWidth)) + 0.5f;
	float targetZ = static_cast<int>(tmp[0] / (mapHeight)) + 0.5f;
	float x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
	float z = this->getPosition().z + (static_cast<float>(mapHeight) / 2);

	int xSign = 0;
	int zSign = 0;
	if (targetX - x < -0.1f)
		xSign = -1;
	else if (targetX - x > 0.1f)
		xSign = 1;
	if (targetZ - z < -0.1f)
		zSign = -1;
	else if (targetZ - z > 0.1f)
		zSign = 1;
	// if (_cooldown <= 0.0f) {
	// 	_cooldown = 0.5f;
	// 	cam->printMapInfo();
	// 	std::cout << x << " " << z << std::endl;
	// 	std::cout << targetX << " " << targetZ << std::endl;
	// 	std::cout << xSign << " " << zSign << std::endl;
	// }
	if (x - (targetX) <= 0.05 && z - (targetZ) <= 0.05) {
		_targetMovement *= 0;
		tmp.erase(tmp.begin());
	}
	else if (xSign != 0 || zSign != 0) {
		float xDirection = static_cast<float>(xSign);
		float zDirection = static_cast<float>(zSign);
		xSign = abs(xSign);
		zSign = abs(zSign);
		float totalMagnitude = xSign + zSign;
		xDirection *= sqrt(xSign / totalMagnitude);
		zDirection *= sqrt(zSign / totalMagnitude);
		_cooldown -= _gameEngine->getDeltaTime();
		float deltaTime = _gameEngine->getDeltaTime();
		_targetMovement.x = xDirection * _speed * deltaTime;
		_targetMovement.z = zDirection * _speed * deltaTime;
	}
	_cooldown -= _gameEngine->getDeltaTime();
}
