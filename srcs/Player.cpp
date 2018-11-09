#include "Player.hpp"

Player::Player(void) {}

Player::Player(Player const &src) { *this = src; }

Player::~Player(void) {}

Player &Player::operator=(Player const &rhs)
{
	return *this;
	(void)rhs;
}

void Player::Update(void)
{
	// std::cout << "yo" << std::endl;
}
