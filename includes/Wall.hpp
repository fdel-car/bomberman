#pragma once

#include "AEntity.hpp"

class Wall : public AEntity {
   public:
	Wall(std::vector<float> position);
	~Wall(void);
	Wall(Wall const &src);
	Wall &operator=(Wall const &rhs);

	virtual void Update(void);

   private:
	Wall(void);
};