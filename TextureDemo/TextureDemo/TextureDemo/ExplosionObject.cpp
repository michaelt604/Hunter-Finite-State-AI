#include "ExplosionObject.h"


ExplosionObject::ExplosionObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityRotation, entityScale, entityTexture, entityNumElements)
{
}

// Renders the GameObject using a shader
void ExplosionObject::render(Shader &shader, std::vector<GameObject*> &gmObjects, double deltaTime) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// TODO: Add different types of transformations
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	// Set the transformation matrix in the shader
	// TODO: Multiply your new transformations to make the transformationMatrix
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
