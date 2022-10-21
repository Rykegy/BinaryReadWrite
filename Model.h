#pragma once
#include <glm.hpp>
#include <glew.h>
#include "Mesh.h"
#include <gtc/matrix_transform.hpp>


namespace HARIKEN {

	class Model
	{
	public:

		Model(glm::vec3 position_ = glm::vec3(), float angle = 0.0f, glm::vec3 rotation_ = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f));
		virtual ~Model();
		void render();
		void addMesh(Mesh* mesh_);

		void setPosition(glm::vec3 position_);
		void setAngle(float angle_);
		void setRotation(glm::vec3 rotation_);
		void setScale(glm::vec3 scale_);

		void deleteTextures();

		meshProperties* getMeshProperties();

		glm::vec3 getPosition() const;
		float getAngle() const;
		glm::vec3 getRotation() const;
		glm::vec3 getScale() const;

		std::vector<vertex> getVertices();
		glm::mat4 getTransform();

	protected:
		std::vector<Mesh*>subMeshes;
		GLuint shaderProgram;

		glm::vec3 position;
		float angle;
		GLfloat opaqueness;
		glm::vec3 rotation;
		glm::vec3 scale = glm::vec3(1.0f);

		meshProperties properties = meshProperties();

	};

}