/***************************************************
Used to store mesh data such as vertices and color in a model.
***************************************************/

#pragma once
#include <vector>
#include <glm.hpp>
#include <glew.h>

namespace HARIKEN {

	struct vertex
	{

		vertex() : position(glm::vec3(0.0f, 0.0f, 0.0f)), normal(glm::vec3(0.0f, 0.0f, 0.0f)), texture(glm::vec3(0.0f, 0.0f, 0.0f)), color(glm::vec3(1.0f, 1.0f, 1.0f)) {}

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture;
		glm::vec3 color;

	};

	struct mtl {

		mtl() : diffMap(0), Ns(0.0f), d(0.0f), Tr(0.0f), Tf(glm::vec3(1.0f)), Ka(glm::vec3(1.0f)), Kd(glm::vec3(1.0f)), Ks(glm::vec3(1.0f)), Ke(glm::vec3(1.0f)) {}

		GLuint diffMap;

		float Ns;
		float Ni;
		float d;
		float Tr;
		glm::vec3 Tf;
		glm::vec3 Ka;
		glm::vec3 Kd;
		glm::vec3 Ks;
		glm::vec3 Ke;

	};

	struct meshProperties {

		GLfloat brightness = 1.0f;
		GLfloat opaqueness = 1.0f;
		glm::vec3 color = glm::vec3();
		glm::vec3 lightColor = glm::vec3();

		meshProperties() {};

	};

}