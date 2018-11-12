#include "Player.hpp"
#include "GameEngine.hpp"

Player::Player(std::vector<float> position, std::vector<float> rotation)
	: Entity(position, rotation) {
	_name = "Player";
	_tag = "Player";
	_speed = 8.0f;
}

Player::Player(Player const &src)
	: Entity(src.getPosition(), src.getRotation()) {
	*this = src;
}

Player::~Player(void) {}

Player &Player::operator=(Player const &rhs) {
	return *this;
	(void)rhs;
}

void Player::Update(void) {
	double deltaTime = _gameEngine->getDeltaTime();

	// Update position based on keyboard
	if (_gameEngine->isKeyPressed(KEY_A)) _position[0] -= _speed * deltaTime;
	if (_gameEngine->isKeyPressed(KEY_D)) _position[0] += _speed * deltaTime;
	if (_gameEngine->isKeyPressed(KEY_W)) _position[2] -= _speed * deltaTime;
	if (_gameEngine->isKeyPressed(KEY_S)) _position[2] += _speed * deltaTime;
}
