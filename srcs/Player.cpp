#include "Player.hpp"
#include "GameEngine.hpp"

Player::Player(std::vector<float> position, std::vector<float> rotation)
	: Entity(position, rotation,
			 new Collider(Collider::Circle, 1,
						  1)) {  // TODO: put collider width/height back to 0.5
	_name = "Player";
	_tag = "Player";
	_speed = 8.0f;
}

Player::Player(Player const &src)
	: Entity(src.getPosition(), src.getRotation(),
			 new Collider(Collider::Circle, 1,
						  1)) {  // TODO: put collider width/height back to 0.5
	*this = src;
}

Player::~Player(void) {}

Player &Player::operator=(Player const &rhs) {
	return *this;
	(void)rhs;
}

void Player::Update(void) {
	double deltaTime = _gameEngine->getDeltaTime();
	int xSign = 0;
	int zSign = 0;
	float xDirection = 0.0f;
	// float yDirection = 0.0f;  // Not used since we cannot jump yet
	float zDirection = 0.0f;

	// Update position based on keyboard
	if (_gameEngine->isKeyPressed(KEY_A)) xSign -= 1;
	if (_gameEngine->isKeyPressed(KEY_D)) xSign += 1;
	if (_gameEngine->isKeyPressed(KEY_W)) zSign -= 1;
	if (_gameEngine->isKeyPressed(KEY_S)) zSign += 1;
	if (xSign == 0 && zSign == 0) {
		// TODO: check for joystick input
	} else {
		// Normalize direction
		xDirection = static_cast<float>(xSign);
		zDirection = static_cast<float>(zSign);
		xSign = abs(xSign);
		zSign = abs(zSign);
		float totalMagnitude = abs(xSign) + abs(zSign);
		xDirection *= sqrt(xSign / totalMagnitude);
		zDirection *= sqrt(zSign / totalMagnitude);
	}

	_targetMovement[0] = xDirection * _speed * deltaTime;
	_targetMovement[2] = zDirection * _speed * deltaTime;
}
