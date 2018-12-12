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
	void updateBoneTransforms(double *animTime, std::string &animName,
							  bool loop, float deltaTime, float speed);
	bool isRigged(void) const;
	void addAnimation(std::string const &animPath, std::string const &animName);

   private:
	std::vector<Mesh *> _meshes;
	std::string const _directory;

	std::vector<Joint *> _joints;
	unsigned int _jointIndex = 0;
	bool _rigged = false;
	bool _animated = false;
	std::map<std::string, double> _animLengths;

	Model(void);
	Model(Model const &src);

	aiNode *_findNodeByName(std::string const &name, aiNode *node);
	void _processNode(aiNode *node, const aiScene *scene, glm::mat4 transform);
	Mesh *_processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 transform);
	void _loadDiffuseTexture(TextureInfo &textureInfo, aiMaterial *assimpMat,
							 Material &material);
	static glm::mat4 toGlmMat4(const aiMatrix4x4 &src);
	void _buildSkeletonHierarchy(aiNode *rootNode);

	Model &operator=(Model const &rhs);
};