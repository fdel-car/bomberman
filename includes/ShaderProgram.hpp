#pragma once

#include "header.hpp"

class ShaderProgram {
   public:
	ShaderProgram(std::string const& vertexPath,
				  std::string const& fragmentPath);
	~ShaderProgram(void);

	GLuint getID(void) const;
	void setBool(std::string const& name, bool value) const;
	void setInt(std::string const& name, int value) const;
	void setFloat(std::string const& name, float value) const;
	void setVec3(std::string const& name, glm::vec3 const& value) const;
	void setMat4(std::string const& name, glm::mat4 const& value) const;

   private:
	GLuint _vs;
	GLuint _fs;
	GLuint _ID;

	ShaderProgram(void);
	ShaderProgram(ShaderProgram const& src);

	ShaderProgram& operator=(ShaderProgram const& rhs);

	void _checkCompileErrors(GLuint shader, std::string type);
};