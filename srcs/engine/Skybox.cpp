#include "engine/Skybox.hpp"

extern std::string _assetsDir;

Skybox::Skybox(std::string str)
	: Entity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f), nullptr, "", "Skybox",
			 "Skybox"),
	  nameTextureDir(str) {
	_initSkyboxFaces();
	_initBuffer();
	_initCubeMap();
}

Skybox::~Skybox(void) {}

GLuint const &Skybox::getVAO(void) const { return _skyboxVAO; }

GLuint const &Skybox::getTexture(void) const { return _skyboxTexture; }

void Skybox::update(void) {}

void Skybox::_initSkyboxFaces(void) {
	_faces.push_back(_assetsDir + "skyboxes/" + nameTextureDir + "/right.png");
	_faces.push_back(_assetsDir + "skyboxes/" + nameTextureDir + "/left.png");
	_faces.push_back(_assetsDir + "skyboxes/" + nameTextureDir + "/top.png");
	_faces.push_back(_assetsDir + "skyboxes/" + nameTextureDir + "/bottom.png");
	_faces.push_back(_assetsDir + "skyboxes/" + nameTextureDir + "/front.png");
	_faces.push_back(_assetsDir + "skyboxes/" + nameTextureDir + "/back.png");
}

void Skybox::_initBuffer(void) {
	float _skyboxVertices[] = {
		// positions
		-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
		1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

		-1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
		-1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

		1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
		1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

	glGenVertexArrays(1, &_skyboxVAO);
	glGenBuffers(1, &_skyboxVBO);
	glBindVertexArray(_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_skyboxVertices), &_skyboxVertices,
				 GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *)0);
	glBindVertexArray(0);
}

void Skybox::_initCubeMap(void) {
	glGenTextures(1, &_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxTexture);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < _faces.size(); i++) {
		unsigned char *data =
			stbi_load(_faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
						 height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		} else {
			std::cout << "Cubemap texture failed to load at path: " << _faces[i]
					  << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

