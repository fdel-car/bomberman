#include "engine/Model.hpp"

extern std::string _assetsDir;

Model::Model(std::string const &modelPath)
	: _meshes(std::vector<Mesh *>()),
	  _directory(modelPath.substr(0, modelPath.find_last_of('/'))) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(
		_assetsDir + modelPath, aiProcess_Triangulate | aiProcess_GenNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
		std::cerr << "\033[0;31m:Error:Assimp:\033[0m "
				  << importer.GetErrorString() << std::endl;
		return;
	}
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
			transform * glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y,
								  mesh->mVertices[i].z, 1.0f);
		vertex.position.x = position.x;
		vertex.position.y = position.y;
		vertex.position.z = position.z;
		glm::vec4 normal =
			transform * glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y,
								  mesh->mNormals[i].z, 1.0f);
		vertex.normal.x = normal.x;
		vertex.normal.y = normal.y;
		vertex.normal.z = normal.z;
		if (mesh->mTextureCoords[0])
			vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
										 mesh->mTextureCoords[0][i].y);
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	// Set default material
	material.diffuseColor = glm::vec3(0.64f);
	material.ambientColor = glm::vec3(0.64f);
	material.specularColor = glm::vec3(0.64f);
	material.hasDiffuseTexture = false;
	material.shininess = 64;

	if (mesh->mMaterialIndex < scene->mNumMaterials) {
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

		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString str;
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			glGenTextures(1, &diffuseTexture);
			glBindTexture(GL_TEXTURE_2D, diffuseTexture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			int x, y, n;
			std::string textureName =
				_assetsDir + _directory + '/' + str.C_Str();
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data =
				stbi_load((textureName).c_str(), &x, &y, &n, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA,
							 GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				material.hasDiffuseTexture = true;
			} else {
				std::cerr << "\033[0;33m:Warning:\033[0m "
						  << "Failed to load texture file: " + textureName
						  << std::endl;
				diffuseTexture = -1;
			}
			stbi_set_flip_vertically_on_load(false);
			stbi_image_free(data);
		}
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

void Model::draw(ShaderProgram const &shaderProgram,
				 glm::vec3 const &color) const {
	for (const auto mesh : _meshes) mesh->draw(shaderProgram, color);
}

std::vector<Mesh *> const Model::getMeshes(void) const { return _meshes; }

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