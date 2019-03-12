#include "TargetObject.h"

TargetObject::TargetObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements)
			: GameObject(entityPos, entityRotation, entityScale, entityTexture, entityNumElements)
{
	initRotation = entityRotation;
	targetSpeed = 1.5f;
}


void TargetObject::update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[4], int size, glm::vec3 target)
{
	//Gives the target a velocity so they are moving, they move in a circularish pattern
	//velocity = glm::vec3(targetSpeed * cos(rotation * M_PI / 180) * deltaTime, targetSpeed * sin(rotation * M_PI / 180) * deltaTime, 0);

	//Calculate the new velocity with acceleration
	glm::vec3 v_t = position - target;
	if (glm::length(v_t) < 15 )
	{
		acceleration = accelerationMax * (v_t - velocity) / (sqrt(pow(v_t.x - velocity.x, 2) + pow(v_t.y - velocity.y, 2) + pow(v_t.z - velocity.z, 2)));
		velocity += 1.0f * acceleration * (float)deltaTime;

		float absV = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));

		if (absV > maxSpeed)	//If speed is over our max speed
		{
			velocity = maxSpeed * velocity / absV;
		}
		rotation = atan2(velocity.y, velocity.x) * 180 / M_PI;
	}

	else //Rotate circlish when not within range of player
		velocity = glm::vec3(targetSpeed * cos(rotation * M_PI / 180) * deltaTime, targetSpeed * sin(rotation * M_PI / 180) * deltaTime, 0);
	
	
	
	GameObject::update(deltaTime, gmObjects, textures, size, target);
}