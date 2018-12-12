#include "engine/Model.hpp"

extern std::string _assetsDir;

Model::Model(std::string const &modelPath)
	: _meshes(std::vector<Mesh *>()),
	  _directory(modelPath.substr(0, modelPath.find_last_of('/'))) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(
		_assetsDir + modelPath, aiProcess_Triangulate | aiProcess_GenNormals |
									aiProcess_LimitBoneWeights);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
		std::cerr << "\033[0;31m:Error:Assimp:\033[0m "
				  << importer.GetErrorString() << std::endl;
		return;
	}
	_processNode(scene->mRootNode, scene, glm::mat4(1.0f));
	if (_joints.size() > 0) _rigged = true;
	_buildSkeletonHierarchy(scene->mRootNode);
	if (scene->HasAnimations()) {
		_animated = true;
		std::cout << "Anim count:" << scene->mNumAnimations << std::endl;
		// Only support one animation for now
		for (size_t i = 0; i < 1 /*scene->mNumAnimations*/; i++) {
			aiAnimation *anim = scene->mAnimations[i];
			std::cout << anim->mName.C_Str() << std::endl;
			_animLength = anim->mDuration;
			std::cout << _animLength << std::endl;
			for (size_t j = 0; j < anim->mNumChannels; j++) {
				aiNodeAnim *nodeAnim = anim->mChannels[j];
				Joint *joint = findJointByName(nodeAnim->mNodeName.C_Str());
				if (joint != nullptr) {
					joint->setPositionKeys(nodeAnim->mPositionKeys,
										   nodeAnim->mNumPositionKeys);
					joint->setRotationKeys(nodeAnim->mRotationKeys,
										   nodeAnim->mNumRotationKeys);
					joint->setScalingKeys(nodeAnim->mScalingKeys,
										  nodeAnim->mNumScalingKeys);
				}
			}
		}
	}
}

Model::~Model(void) {
	for (auto mesh : _meshes) delete mesh;
}

void Model::_buildSkeletonHierarchy(aiNode *rootNode) {
	for (auto joint : _joints) {
		aiNode *node = _findNodeByName(joint->name, rootNode);
		joint->localTransform = toGlmMat4(node->mTransformation);
		Joint *parent = findJointByName(node->mParent->mName.C_Str());
		joint->parent = parent;
	}
	updateBoneTransforms(nullptr, 0.0f);
}

void Model::updateBoneTransforms(double *animTime, float deltaTime) {
	if (_animated) {
		*animTime += deltaTime;
		if (*animTime + EPSILON >= _animLength) *animTime = 0.0;
		for (auto joint : _joints) joint->applyAnimationTransform(*animTime);
	}
	for (auto joint : _joints) joint->updateFinalTransform();
}

void Model::initModel(void) {
	// Meshes
	for (auto mesh : _meshes) {
		mesh->setupTexture();
		mesh->setupBuffers();
	}
}

void Model::_loadDiffuseTexture(TextureInfo &textureInfo, aiMaterial *assimpMat,
								Material &material) {
	aiString str;
	assimpMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
	std::string textureName = _assetsDir + _directory + '/' + str.C_Str();
	stbi_set_flip_vertically_on_load(true);
	textureInfo.data = stbi_load((textureName).c_str(), &textureInfo.x,
								 &textureInfo.y, &textureInfo.n, 0);
	if (textureInfo.data) {
		material.hasDiffuseTexture = true;
	} else {
		std::cerr << "\033[0;33m:Warning:\033[0m "
				  << "Failed to load texture file: " + textureName << std::endl;
	}
	stbi_set_flip_vertically_on_load(false);
}

Mesh *Model::_processMesh(aiMesh *mesh, const aiScene *scene,
						  glm::mat4 transform) {
	std::vector<Vertex> vertices;
	// std::vector<unsigned int> indices;
	TextureInfo textureInfo;
	Material material;

	if (mesh->mMaterialIndex < scene->mNumMaterials) {
		aiColor3D color;
		aiMaterial *assimpMat = scene->mMaterials[mesh->mMaterialIndex];
		assimpMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material.ambientColor.x = color.r;
		material.ambientColor.y = color.g;
		material.ambientColor.z = color.b;
		assimpMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.diffuseColor.x = color.r;
		material.diffuseColor.y = color.g;
		material.diffuseColor.z = color.b;
		assimpMat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material.specularColor.x = color.r;
		material.specularColor.y = color.g;
		material.specularColor.z = color.b;
		assimpMat->Get(AI_MATKEY_SHININESS, material.shininess);
		// Load only one texture per mesh
		if (assimpMat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			_loadDiffuseTexture(textureInfo, assimpMat, material);
	}

	if (mesh->HasBones()) transform = glm::mat4(1.0f);
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec4 position =
			transform * glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y,
								  mesh->mVertices[i].z, 1.0f);
		vertex.position.x = position.x;
		vertex.position.y = position.y;
		vertex.position.z = position.z;
		glm::vec4 normal =
			transform * glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y,
								  mesh->mNormals[i].z, 0.0f);
		vertex.normal.x = normal.x;
		vertex.normal.y = normal.y;
		vertex.normal.z = normal.z;
		if (mesh->mTextureCoords[0])
			vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
										 mesh->mTextureCoords[0][i].y);
		vertices.push_back(vertex);
	}

	if (mesh->HasBones()) {
		for (size_t i = 0; i < mesh->mNumBones; i++) {
			int jointID = _jointIndex;
			aiBone *assimpBone = mesh->mBones[i];
			Joint *joint = findJointByName(assimpBone->mName.C_Str());

			if (!joint) {
				_joints.push_back(new Joint(
					assimpBone->mName.C_Str(),
					toGlmMat4(assimpBone->mOffsetMatrix), _jointIndex++));
			} else {
				jointID = joint->index;
			}

			for (size_t j = 0; j < assimpBone->mNumWeights; j++) {
				aiVertexWeight weight = assimpBone->mWeights[j];
				for (size_t k = 0; k < 4;
					 k++) {  // 4 is the number of possible bones per vertex
					if (vertices[weight.mVertexId].jointIds[k] == -1) {
						vertices[weight.mVertexId].jointIds[k] = jointID;
						vertices[weight.mVertexId].weights[k] = weight.mWeight;
						break;
					}
				}
			}
		}
	}
	return new Mesh(textureInfo, vertices, material);
}

aiNode *Model::_findNodeByName(std::string const &name, aiNode *node) {
	if (name.compare(node->mName.C_Str()) == 0) return node;
	for (size_t i = 0; i < node->mNumChildren; i++) {
		aiNode *found = _findNodeByName(name, node->mChildren[i]);
		if (found) return found;
	}
	return nullptr;
}

Joint *Model::findJointByName(std::string const &name) {
	for (auto joint : _joints)
		if (joint->name.compare(name) == 0) return joint;
	return nullptr;
}

void Model::_processNode(aiNode *node, const aiScene *scene,
						 glm::mat4 transform) {
	transform *= toGlmMat4(node->mTransformation);

	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_processMesh(mesh, scene, transform));
	}
	for (size_t i = 0; i < node->mNumChildren; i++) {
		_processNode(node->mChildren[i], scene, transform);
	}
}

void Model::draw(ShaderProgram const &shaderProgram, glm::vec3 const &color) {
	shaderProgram.setBool("rigged", _rigged);
	if (_rigged) {
		for (size_t i = 0; i < 32; i++) {
			shaderProgram.setMat4("boneTransforms[" + std::to_string(i) + "]",
								  _joints.size() > i
									  ? _joints[i]->finalTransform
									  : glm::mat4(1.0f));
		}
	}
	for (const auto mesh : _meshes) {
		if (mesh != nullptr) mesh->draw(shaderProgram, color);
	}
}

std::vector<Mesh *> const Model::getMeshes(void) const { return _meshes; }

bool Model::isRigged(void) const { return _rigged; }

glm::mat4 Model::toGlmMat4(const aiMatrix4x4 &src) {
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