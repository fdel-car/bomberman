#include "LevelOne.hpp"
#include "Player.hpp"

LevelOne::LevelOne(void) : AGameScene() {
	// Center of map should be 0,0,0
	// float blockHalfSize = 0.5f;
	// for (int i = 0; i < 30; i++) {
	// 	// Up side
	// 	AEntity *newMapElem = new Player();
	// }
	// mapEntities.push_back

	std::vector<float> pos(3, 0);
	pos[0] = MAP_SIZE / 2;
	pos[2] = MAP_SIZE / 2;
	std::vector<float> rot(4, 0);
	startEntities.push_back(new Player(pos, rot));
}

LevelOne::LevelOne(LevelOne const &src) : AGameScene() { *this = src; }

LevelOne::~LevelOne(void) {}

LevelOne &LevelOne::operator=(LevelOne const &rhs) {
	return *this;
	(void)rhs;
}

std::string LevelOne::getSceneName(void) { return "LevelOne"; }