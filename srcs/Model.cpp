#include "Model.hpp"
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
		(_assetsDir + "/objs/" + objDirName + '/' + objDirName + ".obj")
			.c_str(),
		(_assetsDir + "/objs/" + objDirName + '/').c_str());

	// Error output
	if (!warn.empty())
		std::cout << "\033[0;33m:WARN:\033[0m " << warn << std::endl;
	if (!err.empty())
		std::cerr << "\033[0;31m:ERROR:\033[0m " << err << std::endl;

	std::vector<std::vector<t_vertex>> meshesVertices(materials.size());
	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv =
				shapes[s].mesh.num_face_vertices[f];  // Will always be 3 since
													  // we triangulate the .obj
			// Per-face material ID
			int materialID = shapes[s].mesh.material_ids[f];
			t_vertex vertex;

			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				vertex.position.x = attrib.vertices[3 * idx.vertex_index + 0];
				vertex.position.y = attrib.vertices[3 * idx.vertex_index + 1];
				vertex.position.z = attrib.vertices[3 * idx.vertex_index + 2];

				vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
				vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
				vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];

				meshesVertices[materialID].push_back(vertex);
			}
			index_offset += fv;
		}
	}

	int idx = 0;
	for (auto vertices : meshesVertices) {
		t_material material;

		material.diffuse =
			glm::vec3(materials[idx].diffuse[0], materials[idx].diffuse[1],
					  materials[idx].diffuse[2]);

		_meshes.push_back(new Mesh(vertices, material));
		idx++;
	}
}

Model::~Model(void) {
	for (auto mesh : _meshes) delete mesh;
}

void Model::draw(void) const {
	for (const auto mesh : _meshes) mesh->draw();
}

std::vector<Mesh *> const Model::getMeshes(void) const { return _meshes; }
