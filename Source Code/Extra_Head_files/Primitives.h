#pragma once

#include <glew.h>   //*Watch out: before glfw!
#include <glfw3.h>

#include <vector>
#include "Vertex.h"


class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
public:
	Primitive(){}
	virtual ~Primitive() {}

	//Functions
	void set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices)
	{
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertices.push_back(vertices[i]);
		}

		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indices.push_back(indices[i]);
		}
	}

	inline Vertex* getVertices() { return this->vertices.data(); }
	inline GLuint* getIndices() { return this->indices.data(); }
	inline const unsigned getNrOfVertices() { return this->vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->indices.size(); }
};

class Triangle : public Primitive
{
public:
	Triangle()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position                          //Color                             //Texcoords                  //Normals                    
		   glm::vec3(-0.5f, 0.5f, 0.f),         glm::vec3(0.0f, 0.f, 1.f),          glm::vec2(0.f, 1.f),		 glm::vec3(0.0f, 0.f, 1.f),
		   glm::vec3(-0.5f, -0.5f, 0.f),        glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(0.f, 0.f),		 glm::vec3(0.0f, 0.f, 1.f),
		   glm::vec3(0.5f, -0.5f, 0.f),         glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(1.f, 0.f),		 glm::vec3(0.0f, 0.f, 1.f),

		   glm::vec3(0.5f, 0.5f, 0.f),          glm::vec3(1.0f, 0.f, 1.f),          glm::vec2(1.f, 1.f),		 glm::vec3(0.0f, 0.f, 1.f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2, //Triangle
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
private:

};

class Quad : public Primitive
{
public:
	Quad()
		: Primitive()
	{	
		Vertex vertices[] =
		{
			//Position                          //Color                             //Texcoords                  //Normals                    
		   glm::vec3(-1.5f, 1.5f, -0.5f),         glm::vec3(0.0f, 0.f, 1.f),          glm::vec2(0.f, 1.f),		 glm::vec3(0.0f, 0.f, 1.f),
		   glm::vec3(-1.5f, -1.5f, -0.5f),        glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(0.f, 0.f),		 glm::vec3(0.0f, 0.f, 1.f),
		   glm::vec3(1.5f, -1.5f, -0.5f),         glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(1.f, 0.f),		 glm::vec3(0.0f, 0.f, 1.f),

		   glm::vec3(1.5f, 1.5f, -0.5f),          glm::vec3(1.0f, 0.f, 1.f),          glm::vec2(1.f, 1.f),		 glm::vec3(0.0f, 0.f, 1.f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2, //Triangle 1
			0, 2, 3  //Triangle 2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
	private:

};

class Cube : public Primitive
{
public:
	Cube()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position                          //Color                             //Texcoords                  //Normals                    
		   //Front
		   glm::vec3(-0.5f, 0.5f, 0.5f),         glm::vec3(0.0f, 0.f, 1.f),          glm::vec2(0.f, 1.f),		 glm::vec3(0.0f, 0.f, 1.f),
		   glm::vec3(-0.5f, -0.5f, 0.5f),        glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(0.f, 0.f),		 glm::vec3(0.0f, 0.f, 1.f),
		   glm::vec3(0.5f, -0.5f, 0.5f),         glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(1.f, 0.f),		 glm::vec3(0.0f, 0.f, 1.f),
		   glm::vec3(0.5f, 0.5f, 0.5f),          glm::vec3(1.0f, 0.f, 1.f),          glm::vec2(1.f, 1.f),		 glm::vec3(0.0f, 0.f, 1.f),
		   //Back
		   glm::vec3(0.7f, 0.7f, 0.f),        glm::vec3(0.0f, 0.f, 1.f),          glm::vec2(0.f, 1.f),		 glm::vec3(0.0f, 0.f, -1.f),
		   glm::vec3(0.7f, -0.7f, 0.f),       glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(0.f, 0.f),		 glm::vec3(0.0f, 0.f, -1.f),
		   glm::vec3(-0.7f, -0.7f, 0.f),        glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(1.f, 0.f),		 glm::vec3(0.0f, 0.f, -1.f),
		   glm::vec3(-0.7f, 0.7f, 0.f),         glm::vec3(1.0f, 0.f, 1.f),          glm::vec2(1.f, 1.f),		 glm::vec3(0.0f, 0.f, -1.f),
		   //Left
		   //glm::vec3(-0.5f, 0.5f, -0.5f),        glm::vec3(0.0f, 0.f, 1.f),          glm::vec2(0.f, 1.f),		 glm::vec3(-1.f, 0.f, 0.f),
		   //glm::vec3(-0.5f, -0.5f, -0.5f),       glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(0.f, 0.f),		 glm::vec3(-1.f, 0.f, 0.f),
		   //glm::vec3(-0.5f, -0.5f, 0.5f),        glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(1.f, 0.f),		 glm::vec3(-1.f, 0.f, 0.f),
		   //glm::vec3(-0.5f, 0.5f, 0.5f),         glm::vec3(1.0f, 0.f, 1.f),          glm::vec2(1.f, 1.f),		 glm::vec3(-1.f, 0.f, 0.f),
		   ////Right
	       //glm::vec3(0.5f, 0.5f, 0.5f),         glm::vec3(0.0f, 0.f, 1.f),          glm::vec2(0.f, 1.f),		 glm::vec3(1.f, 0.f, 0.f),
		   //glm::vec3(0.5f, -0.5f, 0.5f),        glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(0.f, 0.f),		 glm::vec3(1.f, 0.f, 0.f),
		   //glm::vec3(0.5f, -0.5f, -0.5f),         glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(1.f, 0.f),		 glm::vec3(1.f, 0.f, 0.f),
		   //glm::vec3(0.5f, 0.5f, -0.5f),          glm::vec3(1.0f, 0.f, 1.f),          glm::vec2(1.f, 1.f),		 glm::vec3(1.f, 0.f, 0.f),
		   ////Top
		   //glm::vec3(-0.5f, 0.5f, -0.5f),        glm::vec3(0.0f, 0.f, 1.f),          glm::vec2(0.f, 1.f),		 glm::vec3(0.f, 1.f, 0.f),
		   //glm::vec3(-0.5f, 0.5f, 0.5f),         glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(0.f, 0.f),		 glm::vec3(0.f, 1.f, 0.f),
		   //glm::vec3(0.5f, 0.5f, 0.5f),          glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(1.f, 0.f),		 glm::vec3(0.f, 1.f, 0.f),
		   //glm::vec3(0.5f, 0.5f, -0.5f),         glm::vec3(1.0f, 0.f, 1.f),          glm::vec2(1.f, 1.f),		 glm::vec3(0.f, 1.f, 0.f),
		   ////Botton
		   //glm::vec3(-0.5f, -0.5f, 0.5f),       glm::vec3(0.0f, 0.f, 1.f),          glm::vec2(0.f, 1.f),		 glm::vec3(0.f, -1.f, 0.f),
		   //glm::vec3(-0.5f, -0.5f, -0.5f),        glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(0.f, 0.f),		 glm::vec3(0.f, -1.f, 0.f),
		   //glm::vec3(0.5f, -0.5f, -0.5f),         glm::vec3(1.0f, 1.f, 1.f),          glm::vec2(1.f, 0.f),		 glm::vec3(0.f, -1.f, 0.f),
		   //glm::vec3(0.5f, -0.5f, 0.5f),        glm::vec3(1.0f, 0.f, 1.f),          glm::vec2(1.f, 1.f),		 glm::vec3(0.f, -1.f, 0.f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			//Front
			0, 1, 2, //Triangle 1
			0, 2, 3, //Triangle 2
			//Back
			4, 5, 6,
			4, 6, 7,
			//Left
			8, 9, 10,
			8, 10, 11,
			//Right
			12, 13, 14,
			12, 14, 15,
			////Top
			16, 17, 18,
			16, 18, 19,
			////Botton
			//20, 21, 22,
			//20, 22, 23,
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
private:

};