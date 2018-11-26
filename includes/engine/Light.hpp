#pragma once

#include "Entity.hpp"

class Light : public Entity {
   public:
	Light(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
	virtual ~Light(void);

	// virtual void update(void);

	glm::mat4 const &getProjectionMatrix(void) const;

   private:
	glm::vec3 _dir;
	glm::mat4 _projection;

	Light(void);
	Light(Light const &src);

	Light &operator=(Light const &rhs);
};