#include "Player.hpp"
#include "GameEngine.hpp"

Player::Player(glm::vec3 position, glm::vec3 eulerAngles)
	: Entity(position, eulerAngles, new Collider(Collider::Circle, 1, 1),
			 "Player") {  // TODO: put collider width/height back to 0.5
	_name = "Player";
	_tag = "Player";
	_speed = 8.0f;
}

Player::~Player(void) {}

void Player::Update(void) {
	float deltaTime = _gameEngine->getDeltaTime();

	// Update position based on keyboard
	if (_gameEngine->isKeyPressed(KEY_A))
		this->translate(glm::vec3(1.0, 0.0, 0.0) * _speed * deltaTime);
	if (_gameEngine->isKeyPressed(KEY_D))
		this->translate(glm::vec3(-1.0, 0.0, 0.0) * _speed * deltaTime);
	if (_gameEngine->isKeyPressed(KEY_W))
		this->translate(glm::vec3(0.0, 0.0, 1.0) * _speed * deltaTime);
	if (_gameEngine->isKeyPressed(KEY_S))
		this->translate(glm::vec3(0.0, 0.0, -1.0) * _speed * deltaTime);
}
