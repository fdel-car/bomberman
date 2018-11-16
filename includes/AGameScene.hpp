#pragma once

#include "Camera.hpp"

class AGameScene {
   public:
	AGameScene(void);
	virtual ~AGameScene(void) {}
	std::vector<Entity *> const getEntities() const;
	Camera *getCamera() const;
	virtual void load(void) = 0;

   protected:
	std::vector<Entity *> _entities;
	Camera *_camera;

	AGameScene(AGameScene const &src);

	AGameScene &operator=(AGameScene const &rhs);
};
