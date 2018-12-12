#pragma once

#include "engine/Joint.hpp"
#include "engine/Mesh.hpp"

class Model final {
   public:
	Model(std::string const &modelPath);
	virtual ~Model(void);

	std::vector<Mesh *> const getMeshes(void) const;
	void initModel(void);
	void draw(ShaderProgram const &shaderProgram,
			  glm::vec3 const &color = glm::vec3(-1.0f));
	Joint *findJointByName(std::string const &name);
	void updateBoneTransforms(double *animTime, float deltaTime);
	bool isRigged(void) const;

   private:
	std::vector<Mesh *> _meshes;
	std::string const _directory;

	std::vector<Joint *> _joints;
	unsigned int _jointIndex = 0;
	bool _rigged = false;
	bool _animated = false;
	double _animLength;

	Model(void);
	Model(Model const &src);

	aiNode *_findNodeByName(std::string const &name, aiNode *node);
	void _processNode(aiNode *node, const aiScene *scene, glm::mat4 transform);
	Mesh *_processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transform);
	// aiNode *_findNode(aiNode *node, const char *name);
	void _loadDiffuseTexture(TextureInfo &textureInfo, aiMaterial *assimpMat,
							 Material &material);
	static glm::mat4 toGlmMat4(const aiMatrix4x4 &src);
	void _buildSkeletonHierarchy(aiNode *rootNode);

	Model &operator=(Model const &rhs);
};