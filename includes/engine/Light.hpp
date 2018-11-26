#pragma once

#include "Entity.hpp"

class Light : public Entity {
   public:
	Light(glm::vec3 const &pos, glm::vec3 const &lightDir);
	virtual ~Light(void);

	virtual void update(void);

	glm::vec3 const &getDir(void) const;
	glm::vec3 const &getColor(void) const;
	glm::mat4 const &getProjectionMatrix(void) const;
	glm::mat4 const &getViewMatrix(void) const;

   private:
	glm::vec3 _color;
	glm::vec3 _dir;
	glm::mat4 _projection;
	glm::mat4 _view;

	Light(void);
	Light(Light const &src);

	Light &operator=(Light const &rhs);
};