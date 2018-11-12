#include "Player.hpp"
#include "GameLogic.hpp"

Player::Player(std::vector<float> position, std::vector<float> rotation)
	: AEntity(position, rotation) {
	name = "Player";
	tag = "Player";
	_speed = 8.0f;
}

Player::Player(Player const &src) : AEntity(src.position, src.rotation) {
	*this = src;
}

Player::~Player(void) {}

Player &Player::operator=(Player const &rhs) {
	return *this;
	(void)rhs;
}

void Player::Update(void) {
	double deltaTime = _gameLogic->getDeltaTime();
	// Update position based on keyboard
	if (_gameLogic->isKeyPressed(KEY_A)) position[0] -= _speed * deltaTime;
	if (_gameLogic->isKeyPressed(KEY_D)) position[0] += _speed * deltaTime;
	if (_gameLogic->isKeyPressed(KEY_W)) position[2] -= _speed * deltaTime;
	if (_gameLogic->isKeyPressed(KEY_S)) position[2] += _speed * deltaTime;
}
