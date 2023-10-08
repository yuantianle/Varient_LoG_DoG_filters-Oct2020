#pragma once

#include <iostream>

#include <fstream>
#include <string>

#include <glew.h>   //*Watch out: before glfw!
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	//1. Member variables
	GLuint ID;
	const int versionMajor;
	const int versionMinor;

	//2. Private functions
	std::string loadShaderSource(char* fileName)
	{
		std::string temp = "";                                                      //To get each line
		std::string src = "";                                                       //To get the sources

		std::ifstream in_file;

		//1. vertex
		in_file.open(fileName);

		if (in_file.is_open())
		{
			while (std::getline(in_file, temp))
				src += temp + "\n";
		}
		else
		{
			std::cout << "ERROR::SHADER::COULUD_NOT_OPEN_FILE" << fileName << "\n";
		}

		in_file.close();

		std::string versionNr = std::to_string(this->versionMajor) + std::to_string(this->versionMinor) + "0";
		src.replace(src.find("#version"), 12, ("#version " + versionNr));
		//std::cout << src << "\n";

		return src;
	}

	GLuint loadShader(GLenum type, char* fileName)
	{
		char infoLog[512];												   	 //For error checking
		GLint success;

		GLuint shader = glCreateShader(type);                                //Create a shader and get its ID to "shader"
		std::string str_src = this->loadShaderSource(fileName);
		const GLchar* Src = str_src.c_str();
		glShaderSource(shader, 1, &Src, NULL);                                //Let "shader" get the data source
		glCompileShader(shader);                                              //Compile the "shader"

		//Error checking for compliation
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULUD_NOT_COMPILE_SHADER" << fileName << "\n";
			std::cout << infoLog << "\n";
		}

		return shader;
	}

	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
	{
		char infoLog[512];												   	\
		GLint success;

		this->ID = glCreateProgram();

		glAttachShader(this->ID, vertexShader);
		if (geometryShader) glAttachShader(this->ID, geometryShader);
		glAttachShader(this->ID, fragmentShader);

		glLinkProgram(this->ID);

		//Error checking for program building
		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULUD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n";
		}

	}

public:
	//1. Constructors/Destructors
	Shader(const int versionMajor, const int versionMinor, char* vertexFile, char* fragmentFile, char* geometryFile)
		: versionMajor(versionMajor), versionMinor(versionMinor)
	{
		GLuint vertexShader = 0;
		GLuint fragmentShader = 0;
		GLuint geometryShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
		if (geometryFile != "")
			geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

		this->linkProgram(vertexShader, geometryShader, fragmentShader);

		//End
		glUseProgram(0);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);
	}

	~Shader()                                                                  //Destructor function
	{
		glDeleteProgram(this->ID);
	}

	//2. Helper uniform functions
	void use()
	{
		glUseProgram(this->ID);
	}

	void unuse()
	{
		glUseProgram(0);
	} 

	void set1i(GLint value, const GLchar* name)                           //Uniform variables
	{
		this->use();

		glUniform1i(glGetUniformLocation(this->ID, name), value);

		this->unuse();
	}

	void set1f(GLfloat value, const GLchar* name)                           
	{
		this->use();

		glUniform1f(glGetUniformLocation(this->ID, name), value);

		this->unuse();
	}                  

	void setVec2f(glm::fvec2 value, const GLchar* name)
	{
		this->use();

		glUniform2fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(value));

		this->unuse();
	}

	void setVec3f(glm::fvec3 value, const GLchar* name)
	{
		this->use();

		glUniform3fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(value));
		
		this->unuse();
	}

	void setVec4f(glm::fvec4 value, const GLchar* name)
	{
		this->use();

		glUniform4fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(value));

		this->unuse();
	}

	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE)
	{
		this->use();

		glUniformMatrix3fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(value));

		this->unuse();
	}

	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE)
	{
		this->use();

		glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(value));

		this->unuse();
	}
};
