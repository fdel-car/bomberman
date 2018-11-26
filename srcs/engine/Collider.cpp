#include "engine/Collider.hpp"

Collider::Collider(void)
	: shape(Collider::Rectangle), width(1), height(1), isTrigger(false) {}

Collider::Collider(Shape shape, int layerTag, float width, float height,
				   bool isTrigger)
	: shape(shape),
	  layerTag(layerTag),
	  width(width),
	  height(height),
	  isTrigger(isTrigger) {}

Collider::Collider(Collider const &src) { *this = src; }

Collider::~Collider(void) {}

Collider &Collider::operator=(Collider const &rhs) {
	this->shape = rhs.shape;
	this->isTrigger = rhs.isTrigger;
	this->height = rhs.height;
	this->width = rhs.width;
	return *this;
}