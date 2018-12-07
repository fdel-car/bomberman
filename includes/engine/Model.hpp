#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "engine/Animation.hpp"
#include "engine/Joint.hpp"
#include "engine/Mesh.hpp"

class Model final {
   public:
	Model(std::string const &modelPath);
	virtual ~Model(void);

	std::vector<Mesh *> const getMeshes(void) const;
	void draw(ShaderProgram const &shaderProgram,
			  glm::vec3 const &color = glm::vec3(-1.0f));
	Joint *findJointByName(std::string const &name);
	void updateBoneTransforms(void);
	bool isRigged(void) const;

   private:
	std::vector<Mesh *> _meshes;
	std::string const _directory;

	std::vector<Joint *> _joints;
	unsigned int _jointIndex = 0;
	bool _rigged = false;

	const aiAnimation *animation;  // current animation
	std::vector<Animation *> _jointAnimation;

	Model(void);
	Model(Model const &src);

	aiNode *_findNodeByName(std::string const &name, aiNode *node);
	void _processNode(aiNode *node, const aiScene *scene, glm::mat4 transform);
	void _processAnimation(aiAnimation **animations);
	Mesh *_processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transform);
	void _loadDiffuseTexture(GLuint *diffuseTexture, aiMaterial *assimpMat,
							 Material &material);
	void _buildSkeletonHierarchy(aiNode *rootNode);
	static glm::mat4 toGlmMat4(const aiMatrix4x4 &src);
	static glm::vec3 toGlmvec3(const aiVector3D src);

	Model &operator=(Model const &rhs);
};