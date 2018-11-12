#include "Player.hpp"
#include "GameLogic.hpp"

Player::Player(GameLogic *gameLogic, std::vector<float> position,
			   std::vector<float> rotation)
	: AEntity(gameLogic, position, rotation) {
	name = "Player";
	label = "player";
	_speed = 30.0f;
}

Player::Player(Player const &src)
	: AEntity(src.gameLogic, src.position, src.rotation) {
	*this = src;
}

Player::~Player(void) {}

Player &Player::operator=(Player const &rhs) {
	return *this;
	(void)rhs;
}

void Player::Update(void) {
	double deltaTime = static_cast<float>(gameLogic->getDeltaTime());

	// Update position based on keyboard
	if (gameLogic->isKeyPressed(KEY_A)) position[0] -= _speed * deltaTime;
	if (gameLogic->isKeyPressed(KEY_D)) position[0] += _speed * deltaTime;
	if (gameLogic->isKeyPressed(KEY_W)) position[2] -= _speed * deltaTime;
	if (gameLogic->isKeyPressed(KEY_S)) position[2] += _speed * deltaTime;
}
