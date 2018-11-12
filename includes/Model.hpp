#pragma once

#include "header.hpp"

class Model {
   public:
	Model(std::string const &objPath);
	virtual ~Model(void);

	unsigned int getVAO(void) const;

   private:
	Model(void);
	Model(Model const &src);

	Model &operator=(Model const &rhs);

	unsigned int _VAO;
	unsigned int _VBO;
	unsigned int _EBO;
};