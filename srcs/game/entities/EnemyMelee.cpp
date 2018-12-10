#include "game/entities/EnemyMelee.hpp"

EnemyMelee::EnemyMelee(glm::vec3 position, glm::vec3 eulerAngles,
					   std::string modelName, Entity *gameManager,
					   Entity *toSpawn)
	: AEnemy(position, eulerAngles, "EnemyMelee", EnemyMeleeLayer, true,
			 gameManager, modelName, toSpawn) {
	_speed = 2.0f;
}

EnemyMelee::~EnemyMelee(void) {}

void EnemyMelee::update(void) {
	SceneTools *cam = dynamic_cast<SceneTools *>(_sceneManager);
	if (cam == nullptr) {
		std::cerr << "[EnemyMelee] Update has failed" << std::endl;
		return;
	}
	// findBestWay(cam, 3, true, true);  // Run away and put bombs
	findBestWay(cam);  // Just run away
	walk(cam);
}
