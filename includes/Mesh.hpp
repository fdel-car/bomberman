#pragma once

#include "header.hpp"

typedef struct s_material {
	glm::vec3 diffuse;
} t_material;

class Mesh {
   public:
	Mesh(std::vector<t_vertex> const &vertices, t_material const &material);
	virtual ~Mesh(void);

	size_t getSize(void) const;
	void draw(void) const;

	GLuint VAO;
	GLuint VBO;

   private:
	size_t _size;
	t_material _material;

	Mesh(void);
	Mesh(Mesh const &src);

	Mesh &operator=(Mesh const &rhs);
};