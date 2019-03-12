#pragma once

#include "GameObject.h"

class TargetObject : public GameObject
{
public:
	TargetObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	void update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[4], int size, glm::vec3 target) override;

private:
	float initRotation = 0;
	float targetSpeed = 0;

	float maxSpeed = 0.03f;
	float accelerationMax = 0.09f;
};

