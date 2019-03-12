#include "HealthObject.h"

HealthObject::HealthObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityRotation, entityScale, entityTexture, entityNumElements)
{
}

void HealthObject::update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[5], int size, glm::vec3 target) {
	//position = position + glm::vec3(gmObjects[0]->getSpeed() * cos(rotation * M_PI / 180) * deltaTime, gmObjects[0]->getSpeed() * sin(rotation * M_PI / 180) * deltaTime, 0);
}

void HealthObject::render(Shader &shader, std::vector<GameObject*> &gmObjects, double deltaTime)
{
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// TODO: Add different types of transformations

	//Augments the rotation
	rotation += 400 * (float)deltaTime;
	if (rotation > 360)
		rotation -= 360;

	glm::vec3 playerPos = gmObjects[0]->getPosition();
	float healthDistance = 15.0f;


	//Not sure if this is the allowed way to using transformations to allow the orbit, included below in comments is another version which might be technically more correct but has a very noticible flickering associated and seems to run slower
	position = playerPos + glm::vec3(healthDistance * cos(rotation * M_PI / 180) * deltaTime, healthDistance * sin(rotation * M_PI / 180) * deltaTime, 0);	//Acts as translating orbit to match with the player and translate out

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));	//Setting our matrix rotation
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);										//Setting out matrix scale
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);						//Setting out matrix translation

	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;	//Set out transform matrix to be drawn

	/*
	glm::vec3 transPos = glm::vec3(playerPos.x - position.x, playerPos.y - position.y, playerPos.z - position.z);

	//Setting our matrix's
	glm::mat4 translateCenter = glm::translate(glm::mat4(1.0f), transPos);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));	
	glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3((healthDistance * cos(rotation * M_PI / 180) * deltaTime), (healthDistance * sin(rotation * M_PI / 180) * deltaTime), 1));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

	glm::mat4 transformationMatrix = translateCenter * rotationMatrix * translateBack * scaleMatrix;	*/

	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}