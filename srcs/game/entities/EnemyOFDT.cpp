#include "game/entities/EnemyOFDT.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/scenes/SceneTools.hpp"

EnemyOFDT::EnemyOFDT(glm::vec3 position, glm::vec3 eulerAngles, Entity *gameManager)
	: Enemy(position, eulerAngles, gameManager) {
	_speed = 2.0f;
	scale(glm::vec3(0.8, 0.8, 0.8));
}

EnemyOFDT::~EnemyOFDT(void) {}

void EnemyOFDT::update(void) {
	Enemy::update();
}
