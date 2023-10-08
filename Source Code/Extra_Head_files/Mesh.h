#pragma once

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"
#include "Primitives.h"

class Mesh
{
private:
	unsigned nrOfVertices;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO(Primitive* primitive)
	{
		//Set variables
		this->nrOfVertices = primitive->getNrOfVertices();
		this->nrOfIndices = primitive->getNrOfIndices();

		//5. MODEL Mesh
		//5.1 VAO, VBO, EBO: create boxes, which used to hold bunches of data
		glCreateVertexArrays(1, &this->VAO);

		//5.2 GEN VAO AND BIND: activate the VAO and anything we do with any other buffers will bind to this VAO
		glBindVertexArray(this->VAO);

		//5.3 GEN VBO AND BIND AND SEND DATA: store array data into the box
		glGenBuffers(1, &this->VBO);                                                       //Generate a ID for the buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);                                          //Store array of floats into one part of the box(VBO)
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);   //Send the data in VBO box to the graphics card; GL_STATIC_DRAW: how often u wanna modify the data

		//5.3 GEN EBO AND BIND AND SEND DATA: store index data of array into the box 
		GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

		//5.4 SET VERTEX ATTRIBUTE POINTERS AND ENABLE THEM (INPUT ASSEMBLY): to clearify the attribute meaning of the vertex array
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		//last 2: offset of each row; last 1: offset to the next attribute
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Texcoord
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//5.5 BIND VAO 0
		glBindVertexArray(0);
	}

	//void initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices)
	//{
	//	//Set variables
	//	this->nrOfVertices = nrOfVertices;
	//	this->nrOfIndices = nrOfIndices;

	//	//5. MODEL Mesh
	//	//5.1 VAO, VBO, EBO: create boxes, which used to hold bunches of data
	//	glCreateVertexArrays(1, &this->VAO);

	//	//5.2 GEN VAO AND BIND: activate the VAO and anything we do with any other buffers will bind to this VAO
	//	glBindVertexArray(this->VAO);

	//	//5.3 GEN VBO AND BIND AND SEND DATA: store array data into the box
	//	glGenBuffers(1, &this->VBO);                                                       //Generate a ID for the buffer
	//	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);                                          //Store array of floats into one part of the box(VBO)
	//	glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);   //Send the data in VBO box to the graphics card; GL_STATIC_DRAW: how often u wanna modify the data

	//	//5.3 GEN EBO AND BIND AND SEND DATA: store index data of array into the box 
	//	GLuint EBO;
	//	glGenBuffers(1, &EBO);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

	//	//5.4 SET VERTEX ATTRIBUTE POINTERS AND ENABLE THEM (INPUT ASSEMBLY): to clearify the attribute meaning of the vertex array
	//	//Position
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	//	//last 2: offset of each row; last 1: offset to the next attribute
	//	glEnableVertexAttribArray(0);
	//	//Color
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	//	glEnableVertexAttribArray(1);
	//	//Texcoord
	//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	//	glEnableVertexAttribArray(2);
	//	//Texcoord
	//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	//	glEnableVertexAttribArray(3);

	//	//5.5 BIND VAO 0
	//	glBindVertexArray(0);
	//}

	//void initModelMatrix()                                                                  //Initialize the model maxtrix(UNECESSARY, CAN BE TOTALLY ACHIEVED BY USING THE UPDATE FUNCTION)
	//{
	//	//7.1 INIT MATRIX STUFF: Watch out: array will multiply scale firstly, rotate secondly and translate finally
	//	//7.1.0 Set the global original matrix: used to reset the value of matrix in every iterate
	//	this->position = glm::vec3(0.f);
	//	this->rotation = glm::vec3(0.f);
	//	this->scale    = glm::vec3(1.f);

	//	//7.1.1 The word space
	//	this->ModelMatrix = glm::mat4(1.f);                                                  //Make an identity model matrix
	//	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
	//	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
	//	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
	//	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	//	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	//}

	void updateUniforms(Shader* shader)
	{
		shader->setMat4fv(this->ModelMatrix, "ModelMatrix"); 
	}

	void updateModelMatrix()                                                                 //A part of initModelMatrix() 
	{
		//7.1.1 The word space
		this->ModelMatrix = glm::mat4(1.f);                                                  //Make an identity model matrix
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}

public:
	Mesh(Primitive* primitive,
		glm::vec3 position,
		glm::vec3 rotation,
		glm::vec3 scale)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		this->initVAO(primitive);
		this->updateModelMatrix();
	}
	//Mesh(Vertex* vertexArray, const unsigned &nrOfVerteices, 
	//	 GLuint* indexArray, const unsigned &nrOfIndicies, 
	//	 glm::vec3 position, 
	//	 glm::vec3 rotation,	
	//	 glm::vec3 scale)
	//{
	//	this->position = position;
	//	this->rotation = rotation;
	//	this->scale = scale;
	//	this->initVAO(vertexArray, nrOfVerteices, indexArray, nrOfIndicies);
	//	this->updateModelMatrix();
	//}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}

	//Accessors

	//Modifiers
	void setPosition(const glm::vec3 position)
	{
		this->position = position;
	}

	void setRotation(const glm::vec3 rotation)
	{
		this->rotation = rotation;
	}

	void setScale(const glm::vec3 scale)
	{
		this->scale = scale;
	}
	
	//Functions

	void move(const glm::vec3 position)
	{
		this->position += position;
	}

	void rotate(const glm::vec3 rotation)
	{
		this->rotation += rotation;
	}

	void scaleUp(const glm::vec3 scale)
	{
		this->scale += scale;
	}

	void render(Shader* shader)                                                           //Placed in while
	{
		//Update uniforms
		this->updateModelMatrix();
		this->updateUniforms(shader);

		shader->use();

		//8.3.6 BIND VERTEX ARRAY OBJECT: run the shader for each vetex object
		glBindVertexArray(this->VAO);                                                  //PREPARE FOR VERTEXARRAY
		
		//Render
		//8.3.7 DRAW A NEW FRAME
		glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);            //PLACE START DRAWING!
	}

};