#include "game/entities/Enemy.hpp"
#include "engine/GameEngine.hpp"
#include "game/cams/Tools.hpp"

Enemy::Enemy(glm::vec3 position, glm::vec3 eulerAngles, Entity *gameManager)
	: Entity(glm::vec3(position.x, position.y + 0.4f, position.z), eulerAngles,
			 new Collider(Collider::Circle, 0.4f, 0.4f), "Enemy", "Enemy",
			 "Enemy", gameManager) {
	_speed = 4.0f;
	scale(glm::vec3(0.8, 0.8, 0.8));
}

Enemy::~Enemy(void) {}

void Enemy::update(void) {
	Tools *cam = dynamic_cast<Tools *>(_sceneManager);
	if (cam == nullptr) {
		std::cerr << "Update as fail" << std::endl;
		return;
	}
	size_t mapWidth = cam->getMapWidth();
	size_t mapHeight = cam->getMapHeight();

	float x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
	float z = this->getPosition().z + (static_cast<float>(mapHeight) / 2);

	if (cam->getRefreshAI()) {
		_way.clear();
		size_t pos = static_cast<int>(z) * mapHeight + static_cast<int>(x);
		Node currentPos = Node();
		// std::cout << "x = " << static_cast<int>(x) << " z = " << z << std::endl;
		if (cam->getGraphe().find(pos) != cam->getGraphe().end())
			currentPos = *cam->getGraphe().at(pos);
		size_t bestDist = 100;
		while(1) {
			if (currentPos.prevNodesByDist.size() == 0 || bestDist == 1)
				break ;
			for (auto n : currentPos.prevNodesByDist) {
				if (bestDist > n.first)
					bestDist = n.first;
			}
			currentPos = *currentPos.prevNodesByDist[bestDist][0];
			_way.push_back(currentPos.z * mapHeight + currentPos.x);

		}
	}
	else if (_way.size() != 0) {
		float targetX = (static_cast<int>(_way[0]) % mapWidth) + 0.5f;
		float targetZ = static_cast<int>(_way[0] / mapHeight) + 0.5f;
		int xSign = 0;
		int zSign = 0;
		if (targetX - x < -0.05f)
			xSign = -1;
		else if (targetX - x > 0.05f)
			xSign = 1;
		if (targetZ - z < -0.05f)
			zSign = -1;
		else if (targetZ - z > 0.05f)
			zSign = 1;
		if (x - (targetX) <= 0.05 && z - (targetZ) <= 0.05) {
			_targetMovement *= 0;
			_way.erase(_way.begin());
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
}
