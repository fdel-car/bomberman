#pragma once

#include "ShaderProgram.hpp"
#include "header.hpp"

typedef struct s_material {
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
} t_material;

class Mesh {
   public:
	Mesh(std::vector<t_vertex> const &vertices, t_material const &material);
	virtual ~Mesh(void);

	size_t getSize(void) const;
	void draw(ShaderProgram const &shaderProgram) const;

	GLuint VAO;
	GLuint VBO;

   private:
	size_t _size;
	t_material _material;

	Mesh(void);
	Mesh(Mesh const &src);

	Mesh &operator=(Mesh const &rhs);
};