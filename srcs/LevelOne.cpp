#include "LevelOne.hpp"
#include "Player.hpp"
#include "Wall.hpp"

LevelOne::LevelOne(void) : AGameScene() {
	// TODO: Center of map should be 0,0,0

	std::vector<float> pos(3, 0);
	pos[0] = MAP_SIZE / 2;
	pos[2] = MAP_SIZE / 2;
	std::vector<float> rot(4, 0);
	startEntities.push_back(new Player(pos, rot));

	pos[0] = 4;
	pos[2] = 2;
	startEntities.push_back(new Wall(pos));
	// for (int i = 0; i < MAP_SIZE; i++) {
	// 	// Up side
	// 	pos[0] = i;
	// 	pos[2] = 0;
	// 	startEntities.push_back(new Wall(pos));
	// 	// Left side
	// 	pos[0] = 0;
	// 	pos[2] = i;
	// 	startEntities.push_back(new Wall(pos));
	// 	// Down side
	// 	pos[0] = i;
	// 	pos[2] = (MAP_SIZE - 1);
	// 	startEntities.push_back(new Wall(pos));
	// 	// Right side
	// 	pos[0] = (MAP_SIZE - 1);
	// 	pos[2] = i;
	// 	startEntities.push_back(new Wall(pos));
	// }
}

LevelOne::LevelOne(LevelOne const &src) : AGameScene() { *this = src; }

LevelOne::~LevelOne(void) {}

LevelOne &LevelOne::operator=(LevelOne const &rhs) {
	return *this;
	(void)rhs;
}

std::string LevelOne::getSceneName(void) { return "LevelOne"; }