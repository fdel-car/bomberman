#pragma once

#include "AGameScene.hpp"

class Factory : public AGameScene {
   public:
	Factory(void);
	virtual ~Factory(void);

	void load(void);
};