#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader {
public:
	unsigned int ID; // ID value of the shader 

	Shader(const char* vertexPath, const char* fragmentPath) { // constructor
		string vertexCode;
		string fragmentCode;
		ifstream vShaderfile;
		ifstream fShaderfile;
		vShaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // ensure objects are able to throw exceptions
		fShaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vShaderfile.open(vertexPath); // opening files
			fShaderfile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderfile.rdbuf(); // reading input from shader files into stream
			fShaderStream << fShaderfile.rdbuf();
			vShaderfile.close();
			fShaderfile.close();
			vertexCode = vShaderStream.str(); // converting streams into strings
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure &e) {
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str(); // converting strings into c_strings 

		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER); // compiling shader
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); // checking if successfully compiled
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" << infoLog << endl;
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}; 

	void use() {
		glUseProgram(ID);
	};

	void setBool(const string& name, bool val) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) val);
	};
	void setInt(const string& name, int val) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
	};
	void setFloat(const string& name, float val) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
	};

private:
	int success;
	char infoLog[512];
};

#endif
