#pragma once

#include "header.hpp"

class Mesh {
   public:
	Mesh(std::vector<t_vertex> const &vertices);
	virtual ~Mesh(void);

	size_t getSize(void) const;

	GLuint VAO;
	GLuint VBO;

   private:
	size_t _size;

	Mesh(void);
	Mesh(Mesh const &src);

	Mesh &operator=(Mesh const &rhs);
};