#include "game/entities/EnemyRunAway.hpp"

EnemyRunAway::EnemyRunAway(glm::vec3 position, glm::vec3 eulerAngles,
						   Entity *gameManager)
	: AEnemy(position, eulerAngles, "RunAway", EnemyRunAwayLayer, false,
			 gameManager) {
	_speed = 4.0f;
	scale(glm::vec3(0.8, 0.8, 0.8));
}

EnemyRunAway::~EnemyRunAway(void) {}

void EnemyRunAway::update(void) {
	SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);
	if (cam == nullptr) {
		std::cerr << "Update as fail" << std::endl;
		return;
	}
	findBestWay(cam, 3, true, true); // Run away and put bombs
	// findBestWay(cam, 0, true); // Just run away
	walk(cam);
}
