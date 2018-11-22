#include "engine/Mesh.hpp"

Mesh::Mesh(std::vector<t_vertex> const &vertices, t_material const &material)
	: _size(vertices.size()), _material(material) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vertex) * vertices.size(),
				 &vertices.front(), GL_STATIC_DRAW);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
						  (void *)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
						  (void *)offsetof(t_vertex, normal));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh(void) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::draw(ShaderProgram const &shaderProgram) const {
	shaderProgram.setVec3("material.ambientColor", _material.diffuseColor);
	shaderProgram.setVec3("material.diffuseColor", _material.diffuseColor);
	shaderProgram.setVec3("material.specularColor", _material.diffuseColor);
	shaderProgram.setFloat("material.shininess", _material.shininess);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, _size);
}

size_t Mesh::getSize(void) const { return _size; }