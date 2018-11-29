#include "engine/Model.hpp"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

extern std::string _assetsDir;

Model::Model(std::string const &objPath) : _meshes(std::vector<Mesh *>()) {
	Assimp::Importer importer;
	const aiScene *scene =
		importer.ReadFile(_assetsDir + objPath, aiProcess_Triangulate);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
		std::cout << "\033[0;31m:Error:Assimp:\033[0m "
				  << importer.GetErrorString() << std::endl;
		return;
	}

	// Blender fix. TODO: think of something better
	scene->mRootNode->mTransformation = aiMatrix4x4();

	_processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

Model::~Model(void) {
	for (auto mesh : _meshes) delete mesh;
}

Mesh *Model::_processMesh(aiMesh *mesh, const aiScene *scene,
						  glm::mat4 transform) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	GLuint diffuseTexture = -1;
	Material material;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec4 position =
			glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y,
					  mesh->mVertices[i].z, 1.0f) *
			transform;
		vertex.position.x = position.x;
		vertex.position.y = position.y;
		vertex.position.z = position.z;
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
								  mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0])
			vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
										 mesh->mTextureCoords[0][i].y);
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	// Set default material
	material.diffuseColor = glm::vec3(0.5f);
	material.ambientColor = glm::vec3(0.5f);
	material.specularColor = glm::vec3(0.5f);
	material.hasDiffuseTexture = false;
	material.shininess = 64;

	if (mesh->mMaterialIndex >= 0) {
		aiColor3D color;
		aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material.ambientColor.x = color.r;
		material.ambientColor.y = color.g;
		material.ambientColor.z = color.b;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.diffuseColor.x = color.r;
		material.diffuseColor.y = color.g;
		material.diffuseColor.z = color.b;
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material.specularColor.x = color.r;
		material.specularColor.y = color.g;
		material.specularColor.z = color.b;

		mat->Get(AI_MATKEY_SHININESS, material.shininess);
		std::cout << material.shininess << std::endl;
	}
	return new Mesh(vertices, material, diffuseTexture);
}

void Model::_processNode(aiNode *node, const aiScene *scene,
						 glm::mat4 transform) {
	transform *= toGlmMat4(node->mTransformation);

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_processMesh(mesh, scene, transform));
	}
	for (size_t i = 0; i < node->mNumChildren; i++) {
		_processNode(node->mChildren[i], scene, transform);
	}
}

void Model::draw(ShaderProgram const &shaderProgram) const {
	for (const auto mesh : _meshes) mesh->draw(shaderProgram);
}

std::vector<Mesh *> const Model::getMeshes(void) const { return _meshes; }
