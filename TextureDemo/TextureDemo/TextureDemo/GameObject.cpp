#include "GameObject.h"
#define M_PI

/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

GameObject::GameObject(glm::vec3 &entityPosition, float entityRotation, glm::vec3 &entityScale, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	scale = entityScale;
	rotation = entityRotation;
	texture = entityTexture;
	numElements = entityNumElements;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

// Updates the GameObject's state. Can be overriden for children
void GameObject::update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[5], int size, glm::vec3 target) {
	//Updates spotion based on velocity
	position = position + velocity;

}

// Renders the GameObject using a shader
void GameObject::render(Shader &shader, std::vector<GameObject*> &gmObjects, double deltaTime) {
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

float GameObject::shotExpired() { return 0; }	//just here for virtualness

void GameObject::deleteBullet() { }			//Here for virtualness
