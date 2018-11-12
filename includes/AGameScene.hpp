#ifndef AGAMESCENE_HPP
#define AGAMESCENE_HPP

#include "Entity.hpp"
#include "header.hpp"

class AGameScene {
   public:
	AGameScene(void);
	AGameScene(AGameScene const &src);
	virtual ~AGameScene(void);

	AGameScene &operator=(AGameScene const &rhs);

	virtual std::string getSceneName(void) = 0;

	std::vector<Entity *> startEntities;
};

#endif