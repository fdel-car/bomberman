#pragma once

#include "Entity.hpp"
#include "header.hpp"

class AGameScene {
   public:
	AGameScene(void);
	virtual ~AGameScene(void) {}
	SceneData const getData() const;
	virtual void load(void) = 0;

   protected:
	SceneData _vector;

	AGameScene(AGameScene const &src);

	AGameScene &operator=(AGameScene const &rhs);
};