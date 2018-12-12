#include "engine/Mesh.hpp"

Mesh::Mesh(TextureInfo textureInfo, std::vector<Vertex> vertices,
		   Material const &material)
	: _size(vertices.size()),
	  _textureInfo(textureInfo),
	  _vertices(vertices),
	  _material(material) {}

Mesh::~Mesh(void) {
	if (_textureInfo.data != nullptr) stbi_image_free(_textureInfo.data);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::setupBuffers(void) {
	if (_vertices.size() == 0) return;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _size, &_vertices.front(),
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

	// Joint ids
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex),
						   (void *)offsetof(Vertex, jointIds));
	glEnableVertexAttribArray(3);

	// Weights
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  (void *)offsetof(Vertex, weights));
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Clear vertices
	_vertices.clear();
}

void Mesh::setupTexture(void) {
	if (_textureInfo.data == nullptr) return;

	glGenTextures(1, &_diffuseTexture);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _textureInfo.x, _textureInfo.y, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, _textureInfo.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(_textureInfo.data);
	_textureInfo.data = nullptr;
}

void Mesh::draw(ShaderProgram const &shaderProgram,
				glm::vec3 const &color) const {
	if (color.x != -1.0f && color.y != -1.0f && color.z != -1.0f) {
		shaderProgram.setVec3("material.ambientColor",
							  glm::mix(_material.ambientColor, color, 0.5));
		shaderProgram.setVec3("material.diffuseColor",
							  glm::mix(_material.diffuseColor, color, 0.5));
		shaderProgram.setVec3("material.specularColor",
							  glm::mix(_material.specularColor, color, 0.5));
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