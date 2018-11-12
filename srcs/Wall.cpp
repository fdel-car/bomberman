#include "Wall.hpp"
#include "GameEngine.hpp"

Wall::Wall(std::vector<float> position)
	: Entity(position, std::vector<float>(4, 0),
			 Collider(Collider::Rectangle, 0.5f, 0.5f)) {
	_name = "Wall";
	_tag = "Wall";
}

Wall::Wall(Wall const &src)
	: Entity(src.getPosition(), src.getRotation(),
			 Collider(Collider::Rectangle, 0.5f, 0.5f)) {
	*this = src;
}

Wall::~Wall(void) {}

Wall &Wall::operator=(Wall const &rhs) {
	return *this;
	(void)rhs;
}

void Wall::Update(void) {
	// Wall objs do nothing during updates
}
