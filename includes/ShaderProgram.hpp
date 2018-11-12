#pragma once

#include "header.hpp"

class ShaderProgram {
   public:
	ShaderProgram(std::string const &vsPath, std::string const &fsPath);
	virtual ~ShaderProgram(void);

	unsigned int getProgram(void) const;

   private:
	unsigned int _shaderProgram;

	ShaderProgram(void);
	ShaderProgram(ShaderProgram const &src);

	ShaderProgram &operator=(ShaderProgram const &rhs);
};