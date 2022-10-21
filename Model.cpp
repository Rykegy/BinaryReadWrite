#include "stdafx.h"
#include "Model.h"

using namespace HARIKEN;

Model::Model(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_)
{

	position = position_;
	angle = angle_;
	rotation = rotation_;
	scale = scale_;
	//textureID = textureID_;

}


Model::~Model()
{

	for (auto m : subMeshes) {

		delete m;
		m = nullptr;

	}

	subMeshes.clear();
	subMeshes.shrink_to_fit();

}

void Model::addMesh(Mesh* mesh_) {

	subMeshes.push_back(mesh_);

}

void Model::setPosition(glm::vec3 position_)
{

	position = position_;

}

void Model::setAngle(float angle_)
{

	angle = angle_;

}

void Model::setRotation(glm::vec3 rotation_)
{

	rotation = rotation_;

}

void Model::setScale(glm::vec3 scale_)
{

	scale = scale_;

}

void HARIKEN::Model::deleteTextures()
{

	for (auto m : subMeshes) {
		GLuint textureID = m->getTextureID();
		if(textureID != NULL)
			glDeleteTextures(1, &textureID);
	}

}

meshProperties * HARIKEN::Model::getMeshProperties()
{
	return &properties;
}

glm::mat4 Model::getTransform()
{

	glm::mat4 model = glm::mat4();
	model = glm::translate(model, position);
	model = glm::rotate(model, angle, rotation);
	model = glm::scale(model, scale);

	return model;
}

void Model::render() {

	for (auto m : subMeshes)
		m->render(getTransform(), properties);

}

glm::vec3 Model::getPosition() const
{
	return position;
}

float Model::getAngle() const
{
	return angle;
}

glm::vec3 Model::getRotation() const
{
	return rotation;
}

glm::vec3 Model::getScale() const
{
	return scale;
}

std::vector<vertex> HARIKEN::Model::getVertices()
{

	std::vector<vertex> vList;

	for (auto mesh : subMeshes) {

		for (auto v : mesh->vertices)
			vList.push_back(v);

	}

	return vList;

}
