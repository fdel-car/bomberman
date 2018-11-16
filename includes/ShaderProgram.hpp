#pragma once

#include "header.hpp"

class ShaderProgram {
   public:
	ShaderProgram(std::string const& vertexPath,
				  std::string const& fragmentPath);
	~ShaderProgram(void);

	GLuint getID(void) const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

   private:
	GLuint _vs;
	GLuint _fs;
	GLuint _ID;

	ShaderProgram(void);
	ShaderProgram(ShaderProgram const& src);

	ShaderProgram& operator=(ShaderProgram const& rhs);

	void _checkCompileErrors(GLuint shader, std::string type);
};