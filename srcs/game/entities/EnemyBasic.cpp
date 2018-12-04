#include "game/entities/EnemyBasic.hpp"

EnemyBasic::EnemyBasic(glm::vec3 position, glm::vec3 eulerAngles,
					   Entity *sceneManager)
	: AEnemy(position, eulerAngles, "Basic", EnemyBasicLayer, true,
			 sceneManager) {
	_speed = 2.0f;
	scale(glm::vec3(0.8, 0.8, 0.8));
}

EnemyBasic::~EnemyBasic(void) {}

void EnemyBasic::update(void) {
	SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);
	if (cam == nullptr) {
		std::cerr << "[EnemyBasic] Update has failed." << std::endl;
		return;
	}
	randomMove(cam, 1.0f);
	walk(cam);
}
