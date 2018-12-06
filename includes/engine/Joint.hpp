#pragma once

#include "engine/Engine.hpp"

class Joint {
   public:
	Joint(void);

   private:
	virtual ~Joint(void);

	Joint(Joint const &src);

	Joint &operator=(Joint const &rhs);
};