#pragma once

#include "Camera.hpp"

class AGameScene {
   public:
	AGameScene(void);
	virtual ~AGameScene(void) {}
	SceneData const getData() const;
	Camera const *getCamera() const;
	virtual void load(void) = 0;

   protected:
	SceneData _vector;
	Camera *_camera;

	AGameScene(AGameScene const &src);

	AGameScene &operator=(AGameScene const &rhs);
};