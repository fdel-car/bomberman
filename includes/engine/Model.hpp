#pragma once

#include <assimp/scene.h>
#include "engine/Mesh.hpp"

// TODO: Move that so there is no function instantiation inside a .hpp
inline glm::mat4 toGlmMat4(const aiMatrix4x4 &src) {
	glm::mat4 dest;

	dest[0][0] = src.a1;
	dest[0][1] = src.b1;
	dest[0][2] = src.c1;
	dest[0][3] = src.d1;
	dest[1][0] = src.a2;
	dest[1][1] = src.b2;
	dest[1][2] = src.c2;
	dest[1][3] = src.d2;
	dest[2][0] = src.a3;
	dest[2][1] = src.b3;
	dest[2][2] = src.c3;
	dest[2][3] = src.d3;
	dest[3][0] = src.a4;
	dest[3][1] = src.b4;
	dest[3][2] = src.c4;
	dest[3][3] = src.d4;
	return dest;
}

class Model {
   public:
	Model(std::string const &objPath);
	virtual ~Model(void);

	std::vector<Mesh *> const getMeshes(void) const;
	void draw(ShaderProgram const &shaderProgram) const;

   private:
	std::vector<Mesh *> _meshes;

	Model(void);
	Model(Model const &src);

	void _processNode(aiNode *node, const aiScene *scene, glm::mat4 transform);
	Mesh *_processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transform);

	Model &operator=(Model const &rhs);
};