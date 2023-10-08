#pragma once

#include <glew.h>   //*Watch out: before glfw!
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"  //Will use some part of shader

class Material
{
private:
	glm::vec3 ambient;                                               //FOR MODIFYING
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;                                                //COLOR DATA 
	GLint specularTex;
public:

	Material(glm::vec3 ambient,	glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	~Material()
	{

	}

	//Help functions
	void sendToShader(Shader &program)                                //Some uniform variables
	{
		program.setVec3f(this->ambient, "material.ambient");
		program.setVec3f(this->diffuse, "material.diffuse");
		program.setVec3f(this->specular, "material.specular");
		program.set1i(this->diffuseTex, "material.diffuseTex");
		program.set1i(this->specularTex, "material.specularTex");
	}
};