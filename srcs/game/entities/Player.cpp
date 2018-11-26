#include "game/entities/Player.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/scenes/SceneTools.hpp"

Player::Player(glm::vec3 position, glm::vec3 eulerAngles, Save &save,
			   Entity *sceneManager)
	: Damageable(
		  glm::vec3(position.z, position.y + 0.45f, position.z), eulerAngles,
		  new Collider(Collider::Circle, LayerTag::PlayerLayer, 0.45f, 0.45f),
		  "Player", "Player", "Player", 3, PlayerLayer, PlayerSpecialLayer,
		  2.0f, sceneManager),
	  _save(save),
	  _speed(6.0f),
	  _maxBombs(3),
	  _bombCooldown(10.0f),
	  _bombRange(2),
	  _bombTimers(std::vector<float>()) {
	scale(glm::vec3(0.9, 0.9, 0.9));
}

Player::~Player(void) {}

void Player::update(void) {
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
		SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);
		if (cam != nullptr) {
			if (cam->putBomb(getPosition().x, getPosition().z, _bombCooldown,
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
