#include "engine/Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader/tiny_obj_loader.h"

extern std::string _assetsDir;

Model::Model(std::string const &objDirName) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	tinyobj::LoadObj(
		&attrib, &shapes, &materials, &warn, &err,
		(_assetsDir + "objs/" + objDirName + '/' + objDirName + ".obj").c_str(),
		(_assetsDir + "objs/" + objDirName + '/').c_str());

	// Error output
	if (!warn.empty())
		std::cout << "\033[0;33m:Warning:\033[0m " << warn << std::endl;
	if (!err.empty())
		std::cerr << "\033[0;31m:Error:\033[0m " << err << std::endl;

	std::vector<std::vector<Vertex>> meshesVertices(materials.size());
	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv =
				shapes[s].mesh.num_face_vertices[f];  // Will always be 3 since
													  // we triangulate the .obj
			// Per-face material ID
			int materialID = shapes[s].mesh.material_ids[f];
			Vertex vertex;

			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				vertex.position.x = attrib.vertices[3 * idx.vertex_index + 0];
				vertex.position.y = attrib.vertices[3 * idx.vertex_index + 1];
				vertex.position.z = attrib.vertices[3 * idx.vertex_index + 2];

				vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
				vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
				vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];

				if (!attrib.texcoords.empty()) {
					vertex.texCoords.x =
						attrib.texcoords[2 * idx.texcoord_index + 0];
					vertex.texCoords.y =
						attrib.texcoords[2 * idx.texcoord_index + 1];
				}

				meshesVertices[materialID].push_back(vertex);
			}
			index_offset += fv;
		}
	}

	int idx = 0;
	for (auto vertices : meshesVertices) {
		Material material;

		material.ambientColor =
			glm::vec3(materials[idx].ambient[0], materials[idx].ambient[1],
					  materials[idx].ambient[2]);
		material.diffuseColor =
			glm::vec3(materials[idx].diffuse[0], materials[idx].diffuse[1],
					  materials[idx].diffuse[2]);
		material.specularColor =
			glm::vec3(materials[idx].specular[0], materials[idx].specular[1],
					  materials[idx].specular[2]);
		material.shininess = materials[idx].shininess;
		material.isTextured = false;

		unsigned int diffuseTexture = -1;
		if (!materials[idx].diffuse_texname.empty()) {
			glGenTextures(1, &diffuseTexture);
			glBindTexture(GL_TEXTURE_2D, diffuseTexture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			int x, y, n;
			std::string textureName = _assetsDir + "objs/" + objDirName + '/' +
									  materials[idx].diffuse_texname;
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data =
				stbi_load((textureName).c_str(), &x, &y, &n, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA,
							 GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				material.isTextured = true;
			} else {
				std::cerr << "Failed to load texture: " + textureName
						  << std::endl;
				diffuseTexture = -1;
			}
			stbi_set_flip_vertically_on_load(false);
			stbi_image_free(data);
		}
		_meshes.push_back(new Mesh(vertices, material, diffuseTexture));
		idx++;
	}
}

Model::~Model(void) {
	for (auto mesh : _meshes) delete mesh;
}

void Model::draw(ShaderProgram const &shaderProgram) const {
	for (const auto mesh : _meshes) mesh->draw(shaderProgram);
}

std::vector<Mesh *> const Model::getMeshes(void) const { return _meshes; }
