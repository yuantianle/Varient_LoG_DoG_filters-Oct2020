#pragma once

#include <iostream>

#include <string>

#include <glew.h>   //*Watch out: before glfw!
#include <glfw3.h>

#include <SOIL2.h>   //for picture loading (sometimes another choice is stb)

class Texture
{
private:
	GLuint ID;                                                         //ID is original texture0
	int height;
	int width;
	unsigned int type;
	GLint textureUnit;

public:


	Texture(const char* fileName, GLenum type, GLint texture_unit)     //type is original GL_TEXTURE_2D
	{

		this->type = type;
		this->textureUnit = texture_unit;
			
		//6. TEXTURE INIT 1------------------------------------------------------------
		//HAS BEEN SUBSTITUTE BY THIS POINTER
		//6.1 GENERTATE A TEXTURE
		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);
                                                          
		glGenTextures(1, &this->ID);                                        //Generate one texture unit: texture0
		glBindTexture(type, this->ID);                                      //Work with this texture0

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);                //To solve the problem unfit texture: S is X axis, 
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);                //T is Y axis
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);//When camera closer or picture larger, what kind of filter want(also apply to the Mip Map)
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);            //When camera distant or picture smaller, we do not use filter for Mid Map

		//6.2 CHECK THE IMAGE WAS LOADED CORRECTLY
		if (image)
			{
				glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);//Do the textrue
				glGenerateMipmap(type);                                         //Generate a Mip Map: is the same image with several different resolutions and sizes
			}
		else
			{
			std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
			}

		glActiveTexture(0);
		glBindTexture(type, 0);
		SOIL_free_image_data(image);
	}

	~Texture()
	{
		glDeleteTextures(1, &this->ID);
	}

	inline GLuint getID() const { return this->ID; }

	//void bind(GLint texture_unit, GLenum type) 
	//{
	//	glActiveTexture(GL_TEXTURE0 + texture_unit);
	//	glBindTexture(type, this->ID);
	//}
	void bind()
	{
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->type, this->ID);
	}

	//void unbind(GLenum type)
	//{
	//	glActiveTexture(0);
	//	glBindTexture(type, 0);
	//}
	void unbind()
	{
		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}

	GLint getTextureUnit() const { return this->textureUnit; }

	void loadFromFile(const char* fileName)                                      //Unrequired
	{
		if (this->ID)
		{
			glDeleteTextures(1, &this->ID);
		}

		//6. TEXTURE INIT 1------------------------------------------------------------
		//HAS BEEN SUBSTITUTE BY THIS POINTER
		//6.1 GENERTATE A TEXTURE
		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &this->ID);                                              //Generate one texture unit: texture0
		glBindTexture(this->type, this->ID);                                      //Work with this texture0

		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);                //To solve the problem unfit texture: S is X axis, 
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);                //T is Y axis
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);//When camera closer or picture larger, what kind of filter want(also apply to the Mip Map)
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);            //When camera distant or picture smaller, we do not use filter for Mid Map

		//6.2 CHECK THE IMAGE WAS LOADED CORRECTLY
		if (image)
		{
			glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);//Do the textrue
			glGenerateMipmap(this->type);                                         //Generate a Mip Map: is the same image with several different resolutions and sizes
		}
		else
		{
			std::cout << "ERROR::LOADFROMFILE::TEXTURE_LOADING_FAILED" << "\n";
		}

		glActiveTexture(0);
		glBindTexture(this->type, 0);
		SOIL_free_image_data(image);
	}
};
