#ifndef LEVELONE_HPP
#define LEVELONE_HPP

#include "AGameScene.hpp"

#define MAP_SIZE 40

class LevelOne : public AGameScene {
   public:
	LevelOne(void);
	LevelOne(LevelOne const &src);
	virtual ~LevelOne(void);

	LevelOne &operator=(LevelOne const &rhs);

	virtual std::string getSceneName(void);
};

#endif