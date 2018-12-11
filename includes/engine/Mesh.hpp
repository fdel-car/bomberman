#pragma once

#include "engine/Engine.hpp"
#include "engine/ShaderProgram.hpp"

struct Material {
	glm::vec3 ambientColor = glm::vec3(0.64f);
	glm::vec3 diffuseColor = glm::vec3(0.64f);
	glm::vec3 specularColor = glm::vec3(0.64f);
	bool hasDiffuseTexture = false;
	float shininess = 64;
};

struct TextureInfo {
	unsigned char *data = nullptr;
	int x;
	int y;
	int n;
};

class Mesh final {
   public:
	Mesh(TextureInfo textureInfo, std::vector<Vertex> vertices,
		 Material const &material);
	virtual ~Mesh(void);

	size_t getSize(void) const;
	void setupTexture(void);
	void setupBuffers(void);
	void draw(ShaderProgram const &shaderProgram, glm::vec3 const &color,
			  std::vector<glm::vec3> transform) const;

	GLuint VAO;
	GLuint VBO;

   private:
	size_t _size;
	TextureInfo _textureInfo;
	std::vector<Vertex> _vertices;
	Material const _material;
	GLuint _diffuseTexture;

	Mesh(void);
	Mesh(Mesh const &src);

	Mesh &operator=(Mesh const &rhs);
};