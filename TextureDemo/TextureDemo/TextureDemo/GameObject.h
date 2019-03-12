#pragma once

#define _USE_MATH_DEFINES


#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <time.h>
#include <vector>

#include "Math.h"
#include "Shader.h"

using namespace std;

class GameObject {
public:
	GameObject(glm::vec3 &entityPosition, float entityRotation, glm::vec3 &entityScale, GLuint entityTexture, GLint entityNumElements);

	// Updates the GameObject's state. Can be overriden for children
	virtual void update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[5], int size, glm::vec3 target);

	// Renders the GameObject using a shader
	virtual void render(Shader &shader, std::vector<GameObject*> &gmObjects, double deltaTime);

	// Getters
	inline glm::vec3& getPosition() { return position; }
	inline glm::vec3& getVelocity() { return velocity; }
	inline glm::vec3& getAcceleration() { return acceleration; }

	virtual inline float getSpeed() { return objectSpeed; }
	inline int getFrameCount() { return frameCount; }

	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	inline void setVelocity(glm::vec3& newVelocity) { velocity = newVelocity; }
	inline void setAcceleration(glm::vec3& newAcceleration) { acceleration = newAcceleration; }

	inline void setFrameCount(int frames) { frameCount = frames; }
	//inline void setObjSpeed(float newSpeed) { objectSpeed = newSpeed; }

	// Rotation
	inline void setRotation(float newRotation) { rotation = newRotation; }
	inline void addRotation(float rotationAmount) { rotation += rotationAmount; }

	// Collision variables
	float collisionCooldownTime = 0.2f;
	float currentCollisionCooldown = 0.0f;
	bool shouldCollide = true;

	// BulletStuff
	virtual float shotExpired();
	virtual void deleteBullet();

	//Size stuff
	inline float getSize() { return objectSize; }
	inline void setSize(float size) { objectSize = size; }


protected:
	// Object's Transform Variables
	// TODO: Add more transformation variables
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float rotation;

	// Object's details
	GLint numElements;
	float objectSize; // Not currently being used (will be needed for collision detection when objects have a different scale)
	float objectSpeed;


	// Object's texture
	GLuint texture;

	int frameCount = 10;

	void wallCollision();
};