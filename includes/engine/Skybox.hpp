#pragma once

#include "Entity.hpp"

class Skybox : public Entity {
   public:
	// Skybox(glm::vec3 const &pos, glm::vec3 const &SkyboxDir);
	Skybox(std::string);
	virtual ~Skybox(void);

	GLuint const &getVAO(void) const;
	GLuint const &getTexture(void) const;

	void _initCubeMap(void);
	void _initBuffer(void);

   private:
	Skybox(void);
	Skybox(Skybox const &src);

	Skybox &operator=(Skybox const &rhs);

	void _initSkyboxFaces(void);
	void _initData(void);

	std::vector<std::string> _faces;
	std::vector<unsigned char *> _datas;
	int _width;
	int _height;
	std::string nameTextureDir;
	GLuint _skyboxVAO;
	GLuint _skyboxVBO;
	GLuint _skyboxTexture;
};