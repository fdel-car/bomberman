#include "Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader/tiny_obj_loader.h"

extern std::string _assetsDir;

Model::Model(std::string const &objDirName) : _size(0) {
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

	std::vector<float> vertices = std::vector<float>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	// std::cout << shapes.size() << std::endl;
	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv =
				shapes[s].mesh.num_face_vertices[f];  // Will always be 3 since
													  // we triangulate the .obj
			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				// Push positions inside vertices vector
				vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
				vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
				vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

				// Push normals inside vertices vector
				vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
				vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
				vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);

				_size++;
			}
			index_offset += fv;

			// Per-face material
			// std::cout << shapes[s].mesh.material_ids[f] << std::endl;
		}
		// std::cout << s << std::endl;
	}
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
				 &vertices.front(), GL_STATIC_DRAW);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::~Model(void) {
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}

GLuint Model::getVAO(void) const { return _VAO; }
GLuint Model::getVBO(void) const { return _VBO; }
size_t Model::getSize(void) const { return _size; }
