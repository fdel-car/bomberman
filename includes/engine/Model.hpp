#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "engine/Mesh.hpp"

class Model final {
   public:
	Model(std::string const &modelPath);
	virtual ~Model(void);

	std::vector<Mesh *> const getMeshes(void) const;
	void draw(ShaderProgram const &shaderProgram,
			  glm::vec3 const &color = glm::vec3(-1.0f)) const;

   private:
	std::vector<Mesh *> _meshes;
	std::string const _directory;

	Model(void);
	Model(Model const &src);

	void _processNode(aiNode *node, const aiScene *scene, glm::mat4 transform);
	Mesh *_processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transform);
	// aiNode *_findNode(aiNode *node, const char *name);
	void _loadDiffuseTexture(GLuint *diffuseTexture, aiMaterial *assimpMat,
							 Material &material);
	static glm::mat4 toGlmMat4(const aiMatrix4x4 &src);

	Model &operator=(Model const &rhs);
};