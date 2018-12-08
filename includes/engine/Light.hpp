#pragma once

#include "Entity.hpp"

class Light : public Entity {
   public:
	Light(glm::vec2 const &lightPos, glm::vec3 const &lightDir,
		  float gameDuration = 180.0f);
	virtual ~Light(void);

	virtual void update(void);

	glm::vec3 const &getDir(void) const;
	glm::vec3 const &getColor(void) const;
	glm::mat4 const &getProjectionMatrix(void) const;
	glm::mat4 const &getViewMatrix(void) const;

   protected:
	glm::vec3 _target;
	glm::vec3 _color;

   private:
	float _step;
	float _total;

	glm::vec3 _dir;
	glm::vec3 _rotationAxis;
	glm::mat4 _projection;
	glm::mat4 _view;

	Light(void);
	Light(Light const &src);

	Light &operator=(Light const &rhs);
};