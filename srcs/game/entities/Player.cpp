#include "game/entities/Player.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/entities/Bomb.hpp"

extern std::string _assetsDir;

Player::Player(glm::vec3 position, glm::vec3 eulerAngles, Save &save,
			   Entity *sceneManager)
	: Damageable(
		  glm::vec3(position.x, position.y + 0.4f, position.z), eulerAngles,
		  new Collider(Collider::Circle, LayerTag::PlayerLayer, 0.4f, 0.4f),
		  "Player", "Player", "Player", 3, PlayerLayer, PlayerSpecialLayer,
		  2.0f, sceneManager),
	  _save(save),
	  _speed(6.0f),
	  _maxBombs(3),
	  _bombCooldown(3.0f),
	  _bombRange(2),
	  _bombKick(false),
	  _bombTimers(std::vector<float>()),
	  _cam(dynamic_cast<SceneTools *>(_sceneManager)) {
	scale(glm::vec3(0.8, 0.8, 0.8));
	// setColor(glm::vec3(0.2, 0.2, 0.8));
	if (_cam != nullptr) {
		_cam->tellPlayerHp(_hp);
	}

	_neededSounds["defeat_effect"] =
		_assetsDir + "Audio/Sounds/Hero/defeat_effect.wav";
	_neededSounds["defeat_voice"] =
		_assetsDir + "Audio/Sounds/Hero/defeat_voice.wav";
}

Player::~Player(void) {}

void Player::update(void) {
	if (!_alive) return;
	Damageable::update();

	float deltaTime = _gameEngine->getDeltaTime();

	// Refresh cooldown of bombs
	size_t toRemove = 0;
	for (auto &timer : _bombTimers) {
		timer -= deltaTime;
		if (timer <= 0.0f) toRemove++;
	}
	if (toRemove > 0) {
		_bombTimers.erase(_bombTimers.begin(), _bombTimers.begin() + toRemove);
	}

	// Check if new bomb can be spawned
	if (_gameEngine->isKeyJustPressed(KEY_SPACE) &&
		_bombTimers.size() < _maxBombs && _timeDamaged <= 0.0f) {
		if (_cam != nullptr) {
			if (_cam->putBomb(getPosition().x, getPosition().z, _bombCooldown,
							  _bombRange)) {
				_bombTimers.push_back(_bombCooldown);
			}
		}
	}

	// Update position based on keyboard
	int xSign = 0;
	int zSign = 0;
	float xDirection = 0.0f;
	float zDirection = 0.0f;
	if (_gameEngine->isKeyPressed(_save.leftKey)) xSign -= 1;
	if (_gameEngine->isKeyPressed(_save.rightKey)) xSign += 1;
	if (_gameEngine->isKeyPressed(_save.upKey)) zSign -= 1;
	if (_gameEngine->isKeyPressed(_save.downKey)) zSign += 1;
	if (xSign == 0 && zSign == 0) {
		// TODO: check for joystick input
	} else {
		// Normalize direction
		xDirection = static_cast<float>(xSign);
		zDirection = static_cast<float>(zSign);
		xSign = abs(xSign);
		zSign = abs(zSign);
		float totalMagnitude = xSign + zSign;
		xDirection *= sqrt(xSign / totalMagnitude);
		zDirection *= sqrt(zSign / totalMagnitude);
	}

	_targetMovement.x = xDirection * _speed * deltaTime;
	_targetMovement.z = zDirection * _speed * deltaTime;
}

void Player::onTakeDamage(std::vector<std::string> demagingSounds) {
	Damageable::onTakeDamage();
	if (_cam != nullptr) {
		_cam->tellPlayerHp(_hp);
	}

	if (_hp > 0) {
		if (demagingSounds.size() != 0) {
			int randomIdx = rand() % demagingSounds.size();
			_gameEngine->playSound(demagingSounds[randomIdx]);
		}
	} else {
		_gameEngine->playSound("defeat_effect");
		_gameEngine->playSound("defeat_voice");
	}
}

void Player::onDeath(void) {
	// TODO: start death animation
	_collider->layerTag = _baseLayer;
	_targetMovement *= 0;
}

void Player::gotSpeedBoost(float boost) { _speed += boost; }

void Player::gotBombRangeBoost(size_t boost) { _bombRange += boost; }

void Player::gotMaxBombBoost(size_t boost) { _maxBombs += boost; }

void Player::gotBombKickBoost(bool boost) { _bombKick = boost; }

void Player::onCollisionEnter(Entity *entity) {
	if (_bombKick && _gameEngine->isKeyPressed(KEY_LEFT_SHIFT)) {
		Bomb *bomb = dynamic_cast<Bomb *>(entity);
		if (bomb != nullptr) {
			if (_cam != nullptr) {
				float xDistance = bomb->getPosition().x - getPosition().x;
				float zDistance = bomb->getPosition().z - getPosition().z;
				if (abs(xDistance) >= abs(zDistance))
					zDistance = 0.0f;
				else
					xDistance = 0.0f;
				int xSign =
					(xDistance > 0.0f) ? 1 : (xDistance < 0.0f) ? -1 : 0;
				int zSign =
					(zDistance > 0.0f) ? 1 : (zDistance < 0.0f) ? -1 : 0;
				// Check that bomb can actually be pushed in dir
				if (_cam->canPutBomb(bomb->getPosition().x + xSign,
									 bomb->getPosition().z + zSign)) {
					bomb->pushBomb(xSign, zSign, _id);
				}
			}
		}
	}
}
