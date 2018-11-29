#include "game/entities/EnemyPutBomb.hpp"

EnemyPutBomb::EnemyPutBomb(glm::vec3 position, glm::vec3 eulerAngles,
					 Entity *sceneManager)
	: AEnemy(position, eulerAngles, "EPB", EnemyLayer, false, sceneManager) {
	_speed = 3.0f;
	scale(glm::vec3(0.8, 0.8, 0.8));
}

EnemyPutBomb::~EnemyPutBomb(void) {}

void EnemyPutBomb::update(void) {
	SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);
	if (cam == nullptr) {
		std::cerr << "Update as fail" << std::endl;
		return;
	}
	findBestWay(cam, 2, false, true);
	walk(cam);
}
