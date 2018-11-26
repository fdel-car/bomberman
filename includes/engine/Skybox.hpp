#pragma once

#include "Entity.hpp"

class Skybox : public Entity {
   public:
	// Skybox(glm::vec3 const &pos, glm::vec3 const &SkyboxDir);
    Skybox(std::string);
	virtual ~Skybox(void);

	virtual void update(void);

    std::string getTest(void);

   private:
    Skybox(void);
	Skybox(Skybox const &src);

	Skybox &operator=(Skybox const &rhs);

    // unsigned int loadCubemap(std::vector<std::string> faces);

    // std::vector<std::string> faces;

    std::string test;
};