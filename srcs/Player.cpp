#include "Player.hpp"
#include "GameEngine.hpp"

Player::Player(glm::vec3 position, glm::vec3 eulerAngles)
	: Entity(position, eulerAngles, new Collider(Collider::Circle, 1, 1),
			 nullptr,
			 false) {  // TODO: put collider width/height back to 0.5
	_name = "Player";
	_tag = "Player";
	_speed = 8.0f;
}

Player::~Player(void) {}

void Player::Update(void) {
	// double deltaTime = _gameEngine->getDeltaTime();

	// // Update position based on keyboard
	// if (_gameEngine->isKeyPressed(KEY_A)) _position.x -= _speed * deltaTime;
	// if (_gameEngine->isKeyPressed(KEY_D)) _position.x += _speed * deltaTime;
	// if (_gameEngine->isKeyPressed(KEY_W)) _position.z -= _speed * deltaTime;
	// if (_gameEngine->isKeyPressed(KEY_S)) _position.z += _speed * deltaTime;
}
