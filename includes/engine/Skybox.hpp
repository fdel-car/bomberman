#pragma once

#include "Entity.hpp"

// For threads
#include <atomic>
#include <thread>

class Skybox : public Entity {
   public:
	// Skybox(glm::vec3 const &pos, glm::vec3 const &SkyboxDir);
    Skybox(std::string);
	virtual ~Skybox(void);

	virtual void update(void);

    GLuint const &getVAO(void) const;
    GLuint const &getTexture(void) const;

   private:
    Skybox(void);
	Skybox(Skybox const &src);

	Skybox &operator=(Skybox const &rhs);

    void _initSkyboxFaces(void);
    void _initBuffer(void);
	void _initCubeMap(void);

    static std::thread *loadImageThread;

    std::vector<std::string> _faces;
    std::string nameTextureDir;
    GLuint _skyboxVAO;
	GLuint _skyboxVBO;
	GLuint	_skyboxTexture;
};