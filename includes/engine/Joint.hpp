#pragma once

#include "engine/Engine.hpp"

class Joint {
   public:
	Joint(std::string const &name, glm::mat4 const &offsetMatrix, int index);

	virtual ~Joint(void);

	std::string const name;
	glm::mat4 const offsetMatrix;
	int const index;
	Joint *parent;
	glm::mat4 localTransform;
	glm::mat4 finalTransform;

	void updateFinalTransform(void);

   private:
	static glm::mat4 _toYAxisUp;

	Joint(void);
	Joint(Joint const &src);

	Joint &operator=(Joint const &rhs);
};