#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "engine/Engine.hpp"

struct Collider {
   public:
	enum Shape { Rectangle = 0, Circle };
	Collider(void);
	Collider(Shape shape, float width, float height);
	Collider(Collider const &src);
	~Collider(void);

	Shape shape;
	float width;
	float height;

   private:
	Collider &operator=(Collider const &rhs);
};

#endif