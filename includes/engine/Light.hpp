#pragma once

#include "Entity.hpp"

class Light : public Entity {
   public:
	Light(void);
	virtual ~Light(void);

   private:
	Light(Light const &src);

	Light &operator=(Light const &rhs);
};