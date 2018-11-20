#pragma once

#include "Mesh.hpp"

class Model {
   public:
	Model(std::string const &objDirName);
	virtual ~Model(void);

	std::vector<Mesh *> const getMeshes(void) const;
	void draw(ShaderProgram const &shaderProgram) const;

   private:
	Model(void);
	Model(Model const &src);

	Model &operator=(Model const &rhs);

	std::vector<Mesh *> _meshes;
};