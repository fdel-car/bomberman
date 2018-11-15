#pragma once

#include "Entity.hpp"

class Camera : public Entity {
   public:
	Camera(glm::vec3 const &cameraPos, glm::vec3 const &cameraTarget);
	virtual ~Camera(void);

	glm::mat4 const &getViewMatrix(void) const;
	glm::mat4 const &getProjectionMatrix(void) const;

	virtual void update(void);
	void initEntity(GameEngine *gameEngine);

   private:
	float _speed;
	glm::mat4 _view;
	glm::mat4 _projection;

	glm::vec3 _front;

	Camera(void);
	Camera(Camera const &src);

	Camera &operator=(Camera const &rhs);
};