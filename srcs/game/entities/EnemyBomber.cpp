#include "game/entities/EnemyBomber.hpp"

EnemyBomber::EnemyBomber(glm::vec3 position, glm::vec3 eulerAngles,
					   std::string modelName, Entity *gameManager,
					   Entity *toSpawn)
	: AEnemy(position, eulerAngles, "EnemyBomber", EnemyBomberLayer, false,
			 gameManager, modelName, toSpawn) {
	_speed = 3.0f;
}

EnemyBomber::~EnemyBomber(void) {}

void EnemyBomber::update(void) {
	SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);
	if (cam == nullptr) {
		std::cerr << "[EnemyBomber] Update has failed" << std::endl;
		return;
	}
	findBestWay(cam, 2, false, true);
	walk(cam);
}
