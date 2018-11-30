#pragma once

#include "engine/Entity.hpp"

class Portal : public Entity {
   public:
	Portal(glm::vec3 position, Entity *sceneManager);
	virtual ~Portal(void);

	virtual void onTriggerEnter(Entity *entity);

   private:
	bool _hasTriggered;
};
