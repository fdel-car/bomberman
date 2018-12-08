#include "game/entities/EnemyRunAway.hpp"

EnemyRunAway::EnemyRunAway(glm::vec3 position, glm::vec3 eulerAngles, std::string modelName,
						   Entity *gameManager, Entity *toSpawn)
	: AEnemy(position, eulerAngles, "RunAway", EnemyRunAwayLayer, true,
			 gameManager, modelName, toSpawn) {
	_speed = 4.0f;
}

EnemyRunAway::~EnemyRunAway(void) {}

void EnemyRunAway::update(void) {
	SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);
	if (cam == nullptr) {
		std::cerr << "[EnemyRunAway] Update has failed" << std::endl;
		return;
	}
	// findBestWay(cam, 3, true, true);  // Run away and put bombs
	findBestWay(cam, 0, true);  // Just run away
	walk(cam);
}
