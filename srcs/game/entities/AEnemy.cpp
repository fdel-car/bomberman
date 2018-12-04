#include "game/entities/AEnemy.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/entities/Player.hpp"

extern std::string _assetsDir;

AEnemy::AEnemy(glm::vec3 position, glm::vec3 eulerAngles, std::string name,
			   LayerTag tag, bool doMeleeDmg, Entity *sceneManager,
			   Entity *toSpawn)
	: Damageable(glm::vec3(position.x, position.y + 0.4f, position.z),
				 eulerAngles, new Collider(Collider::Circle, tag, 0.45f, 0.45f),
				 "Sphere", name, "Enemy", 1, tag, EnemySpecialLayer, 2.0f,
				 sceneManager),
	  _bombCooldown(0.0f),
	  _resetMoveCoolDown(0.0f),
	  _doMeleeDmg(doMeleeDmg),
	  _hasSpawned(false),
	  _toSpawn(toSpawn),
	  _damagingSounds(std::vector<std::string>()) {
	_neededSounds["damage_1"] = _assetsDir + "Audio/Sounds/Enemy/damage_1.wav";
	_neededSounds["damage_2"] = _assetsDir + "Audio/Sounds/Enemy/damage_2.wav";
	_neededSounds["damage_3"] = _assetsDir + "Audio/Sounds/Enemy/damage_3.wav";
	_damagingSounds.push_back("damage_1");
	_damagingSounds.push_back("damage_2");
	_damagingSounds.push_back("damage_3");
}

AEnemy::~AEnemy(void) {
	if (!_hasSpawned && _toSpawn != nullptr) {
		delete _toSpawn;
		_toSpawn = nullptr;
	}
}

void AEnemy::findBestWay(SceneTools *cam, size_t distFromPlayer, bool runAway,
						 bool putBomb) {
	if (_bombCooldown >= 0.0f && putBomb)
		_bombCooldown -= _gameEngine->getDeltaTime();
	if (cam->getRefreshAI()) {
		_way.clear();
		if (!runAway) {
			_runIn(cam, distFromPlayer, putBomb);
		} else {
			_runAway(cam, distFromPlayer, putBomb);
		}
	}
}

void AEnemy::_runIn(SceneTools *cam, size_t distFromPlayer, bool putBomb) {
	size_t mapWidth = cam->getMapWidth();
	size_t mapHeight = cam->getMapHeight();
	float x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
	float z = this->getPosition().z + (static_cast<float>(mapHeight) / 2);
	size_t bestDist = std::numeric_limits<std::size_t>::max();
	size_t pos = static_cast<int>(z) * mapWidth + static_cast<int>(x);
	if (cam->getGraphe().find(pos) != cam->getGraphe().end()) {
		Node currentPos = *cam->getGraphe().at(pos);
		if (putBomb && _bombCooldown <= 0.0f) {
			if (currentPos.prevNodesByDist.size() == 0) return;
			for (const auto &n : currentPos.prevNodesByDist) {
				if (bestDist > n.first) bestDist = n.first;
			}
			if (bestDist <= distFromPlayer) {
				_bombCooldown = 2.0f;
				cam->putBomb(getPosition().x, getPosition().z, 2.0f, 1);
				_runAway(cam, distFromPlayer, false);
				return;
			}
		}
		if (_bombCooldown >= 0.0f && putBomb) {
			_runAway(cam, distFromPlayer, false);
			return;
		}
		while (1) {
			if (currentPos.prevNodesByDist.size() == 0) return;
			for (const auto &n : currentPos.prevNodesByDist)
				if (bestDist > n.first) bestDist = n.first;
			if (bestDist <= distFromPlayer) {
				_targetMovement *= 0;
				break;
			}
			if (currentPos.prevNodesByDist[bestDist].size() == 0) break;
			if (bestDist != std::numeric_limits<std::size_t>::max())
				currentPos = *currentPos.prevNodesByDist[bestDist][0];
			_way.push_back(currentPos.z * mapWidth + currentPos.x);
		}
	}
}

void AEnemy::_runAway(SceneTools *cam, size_t distFromPlayer, bool putBomb) {
	size_t mapWidth = cam->getMapWidth();
	size_t mapHeight = cam->getMapHeight();
	float x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
	float z = this->getPosition().z + (static_cast<float>(mapHeight) / 2);
	size_t bestDist = std::numeric_limits<std::size_t>::max();
	size_t xPlayer = cam->getRunAwayPos() % mapWidth;
	size_t zPlayer = cam->getRunAwayPos() / mapWidth;
	size_t pos = static_cast<int>(z) * mapWidth + static_cast<int>(x);
	if (cam->getGraphe().find(pos) != cam->getGraphe().end()) {
		Node currentPos = *cam->getGraphe().at(pos);
		if (putBomb && _bombCooldown <= 0.0f)
			for (const auto &n : currentPos.prevNodesByDist)
				if (distFromPlayer >= n.first) {
					_bombCooldown = 2.0f;
					cam->putBomb(getPosition().x, getPosition().z, 2.0f, 1);
				}
		while (1) {
			if (currentPos.runAwayNodesByDist.size() == 0) return;
			if (xPlayer == currentPos.x && zPlayer == currentPos.z) break;
			for (const auto &n : currentPos.runAwayNodesByDist)
				if (bestDist > n.first) bestDist = n.first;
			if (bestDist != std::numeric_limits<std::size_t>::max()) {
				size_t playerDist = 0;
				size_t idx = 0;
				size_t saveIdx = 0;
				for (const auto &v : currentPos.runAwayNodesByDist[bestDist]) {
					if (v->dist > playerDist) {
						playerDist = v->dist;
						saveIdx = idx;
					}
					idx++;
				}
				if (idx == 0) break;
				currentPos = *currentPos.runAwayNodesByDist[bestDist][saveIdx];
			}
			_way.push_back(currentPos.z * mapWidth + currentPos.x);
		}
	}
	// for (const auto &c : _way) {
	// 	std::cout << c << std::endl;
	// }
}

void AEnemy::randomMove(SceneTools *cam, float timer) {
	_resetMoveCoolDown -= _gameEngine->getDeltaTime();
	if (_resetMoveCoolDown <= 0.0f) {
		_way.clear();
		size_t mapWidth = cam->getMapWidth();
		size_t mapHeight = cam->getMapHeight();
		size_t x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
		size_t z = this->getPosition().z + (static_cast<float>(mapHeight) / 2);
		size_t pos;
		if (x > 1) {
			pos = z * mapWidth + (x - 1);
			if (cam->getEntitiesInSquares()[pos].size() == 0) {
				for (size_t tmpX = x - 1; tmpX > 0; tmpX--) {
					pos = z * mapWidth + tmpX;
					_way.push_back(pos);
				}
			}
		}
		if (x < mapWidth - 1) {
			pos = z * mapWidth + (x + 1);
			if (cam->getEntitiesInSquares()[pos].size() == 0) {
				if (_way.size() != 0 && std::rand() % 4 != 0) return;
				_way.clear();
				for (size_t tmpX = x + 1; tmpX < mapWidth - 1; tmpX++) {
					pos = z * mapWidth + tmpX;
					_way.push_back(pos);
				}
			}
		}
		if (z > 1) {
			pos = (z - 1) * mapWidth + x;
			if (cam->getEntitiesInSquares()[pos].size() == 0) {
				if (_way.size() != 0 && std::rand() % 4 != 0) return;
				_way.clear();
				for (size_t tmpZ = z - 1; tmpZ > 0; tmpZ--) {
					pos = tmpZ * mapWidth + x;
					_way.push_back(pos);
				}
			}
		}
		if (z < mapHeight - 1) {
			pos = (z + 1) * mapWidth + x;
			if (cam->getEntitiesInSquares()[pos].size() == 0) {
				if (_way.size() != 0 && std::rand() % 4 != 0) return;
				_way.clear();
				for (size_t tmpZ = z + 1; tmpZ < mapHeight - 1; tmpZ++) {
					pos = tmpZ * mapWidth + x;
					_way.push_back(pos);
				}
			}
		}
		_resetMoveCoolDown = timer;
	}
}

void AEnemy::walk(SceneTools *cam) {
	if (_way.size() != 0) {
		size_t mapWidth = cam->getMapWidth();
		size_t mapHeight = cam->getMapHeight();
		float x = this->getPosition().x + (static_cast<float>(mapWidth) / 2);
		float z = this->getPosition().z + (static_cast<float>(mapHeight) / 2);
		float targetX = (static_cast<int>(_way[0]) % mapWidth) + 0.5f;
		float targetZ = static_cast<int>(_way[0] / mapWidth) + 0.5f;
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
		if (x - targetX <= 0.05 && z - targetZ <= 0.05 &&
			x - targetX >= -0.05 && z - targetZ >= -0.05) {
			_targetMovement *= 0;
			_way.erase(_way.begin());
		} else if (xSign != 0 || zSign != 0) {
			float xDirection = static_cast<float>(xSign);
			float zDirection = static_cast<float>(zSign);
			xSign = abs(xSign);
			zSign = abs(zSign);
			float totalMagnitude = xSign + zSign;
			xDirection *= sqrt(xSign / totalMagnitude);
			zDirection *= sqrt(zSign / totalMagnitude);
			float deltaTime = _gameEngine->getDeltaTime();
			_targetMovement.x = xDirection * _speed * deltaTime;
			_targetMovement.z = zDirection * _speed * deltaTime;
		}
	}
}

void AEnemy::onCollisionEnter(Entity *entity) {
	if (_doMeleeDmg) {
		Player *player = dynamic_cast<Player *>(entity);
		if (player != nullptr) player->takeDamage(_damagingSounds);
	}
}

void AEnemy::onDeath(void) {
	Damageable::onDeath();
	if (_toSpawn != nullptr && !_hasSpawned) {
		_hasSpawned = true;
		_toSpawn->translate(getPosition() - _toSpawn->getPosition());
		_gameEngine->addNewEntity(_toSpawn);
	}
}
