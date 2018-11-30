#include "engine/Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> const &vertices, Material const &material,
		   GLuint diffuseTexture)
	: _size(vertices.size()),
	  _material(material),
	  _diffuseTexture(diffuseTexture) {
	_setupBuffers(vertices);
}

Mesh::~Mesh(void) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::_setupBuffers(std::vector<Vertex> const &vertices) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _size, &vertices.front(),
				 GL_STATIC_DRAW);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  (void *)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  (void *)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::draw(ShaderProgram const &shaderProgram,
				glm::vec3 const &color) const {
	if (color.x != -1.0f && color.y != -1.0f && color.z != -1.0f) {
		shaderProgram.setVec3("material.ambientColor",
							  glm::mix(_material.ambientColor, color, 0.7));
		shaderProgram.setVec3("material.diffuseColor",
							  glm::mix(_material.diffuseColor, color, 0.7));
		shaderProgram.setVec3("material.specularColor",
							  glm::mix(_material.specularColor, color, 0.7));
	} else {
		shaderProgram.setVec3("material.ambientColor", _material.ambientColor);
		shaderProgram.setVec3("material.diffuseColor", _material.diffuseColor);
		shaderProgram.setVec3("material.specularColor",
							  _material.specularColor);
	}
	shaderProgram.setFloat("material.shininess", _material.shininess);
	shaderProgram.setBool("material.hasDiffuseTexture",
						  _material.hasDiffuseTexture);

	if (_material.hasDiffuseTexture) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _diffuseTexture);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, _size);
}

size_t Mesh::getSize(void) const { return _size; }