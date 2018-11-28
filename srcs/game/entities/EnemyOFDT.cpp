#include "game/entities/EnemyOFDT.hpp"


EnemyOFDT::EnemyOFDT(glm::vec3 position, glm::vec3 eulerAngles, Entity *gameManager)
	: AEnemy(position, eulerAngles, "OFDT", EnemyLayer, gameManager) {
	_speed = 2.0f;
	scale(glm::vec3(0.8, 0.8, 0.8));
}

EnemyOFDT::~EnemyOFDT(void) {}

void EnemyOFDT::update(void) {
	SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);
	if (cam == nullptr) {
		std::cerr << "Update as fail" << std::endl;
		return;
	}
	findBestWay(cam);
	walk(cam);
}
