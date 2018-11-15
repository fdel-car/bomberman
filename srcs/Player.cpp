#include "Player.hpp"
#include "GameEngine.hpp"

Player::Player(glm::vec3 position, glm::vec3 eulerAngles)
	: Entity(position, eulerAngles, new Collider(Collider::Circle, 1.0f, 1.0f),
			 "Player") {
	_name = "Player";
	_tag = "Player";
	_speed = 8.0f;
}

Player::~Player(void) {}

void Player::Update(void) {
	float deltaTime = _gameEngine->getDeltaTime();

	// // Update position based on keyboard
	// if (_gameEngine->isKeyPressed(KEY_A))
	// 	this->translate(glm::vec3(1.0, 0.0, 0.0) * _speed * deltaTime);
	// if (_gameEngine->isKeyPressed(KEY_D))
	// 	this->translate(glm::vec3(-1.0, 0.0, 0.0) * _speed * deltaTime);
	// if (_gameEngine->isKeyPressed(KEY_W))
	// 	this->translate(glm::vec3(0.0, 0.0, 1.0) * _speed * deltaTime);
	// if (_gameEngine->isKeyPressed(KEY_S))
	// 	this->translate(glm::vec3(0.0, 0.0, -1.0) * _speed * deltaTime);

	int xSign = 0;
	int zSign = 0;
	float xDirection = 0.0f;
	// float yDirection = 0.0f;  // Not used since we cannot jump yet
	float zDirection = 0.0f;

	// Update position based on keyboard
	if (_gameEngine->isKeyPressed(KEY_D)) xSign -= 1;
	if (_gameEngine->isKeyPressed(KEY_A)) xSign += 1;
	if (_gameEngine->isKeyPressed(KEY_S)) zSign -= 1;
	if (_gameEngine->isKeyPressed(KEY_W)) zSign += 1;
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

	_targetMovement.x = xDirection * _speed * deltaTime;
	_targetMovement.z = zDirection * _speed * deltaTime;
}
