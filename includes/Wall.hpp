#pragma once

#include "Entity.hpp"

class Wall : public Entity {
   public:
	Wall(std::vector<float> position);
	~Wall(void);
	Wall(Wall const &src);
	Wall &operator=(Wall const &rhs);

	virtual void Update(void);

   private:
	Wall(void);
};