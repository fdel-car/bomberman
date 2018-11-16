#pragma once

#include "header.hpp"

class Model {
   public:
	Model(std::string const &objDirName);
	virtual ~Model(void);

	size_t getSize(void) const;
	GLuint getVAO(void) const;
	GLuint getVBO(void) const;

   private:
	Model(void);
	Model(Model const &src);

	Model &operator=(Model const &rhs);

	size_t _size;

	GLuint _VAO;
	GLuint _VBO;
};