#include "engine/ShaderProgram.hpp"

ShaderProgram::ShaderProgram(std::string const& vertexPath,
							 std::string const& fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath.c_str());
		fShaderFile.open(fragmentPath.c_str());
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (const std::ifstream::failure& err) {
		throw(std::runtime_error("Could not read the file " + vertexPath +
								 " or " + fragmentPath + "."));
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vs, 1, &vShaderCode, NULL);
	glCompileShader(_vs);
	_checkCompileErrors(_vs, "VERTEX");

	_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fs, 1, &fShaderCode, NULL);
	glCompileShader(_fs);
	_checkCompileErrors(_fs, "FRAGMENT");

	_ID = glCreateProgram();
	glAttachShader(_ID, _vs);
	glAttachShader(_ID, _fs);
	glLinkProgram(_ID);
	_checkCompileErrors(_ID, "PROGRAM");
}

ShaderProgram::~ShaderProgram(void) {
	glDetachShader(_ID, _vs);
	glDetachShader(_ID, _fs);
	glDeleteShader(_vs);
	glDeleteShader(_fs);
	glDeleteProgram(_ID);
}

void ShaderProgram::_checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr
				<< "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
				<< infoLog
				<< "\n -- "
				   "--------------------------------------------------- -- "
				<< std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr
				<< "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
				<< infoLog
				<< "\n -- "
				   "--------------------------------------------------- -- "
				<< std::endl;
		}
	}
}

void ShaderProgram::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(_ID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(_ID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(_ID, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string& name,
							glm::vec3 const& value) const {
	glUniform3fv(glGetUniformLocation(_ID, name.c_str()), 1,
				 glm::value_ptr(value));
}

void ShaderProgram::setMat4(const std::string& name,
							glm::mat4 const& value) const {
	glUniformMatrix4fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE,
					   glm::value_ptr(value));
}

unsigned int ShaderProgram::getID(void) const { return _ID; }
