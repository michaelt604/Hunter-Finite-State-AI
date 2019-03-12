#pragma once

#include "GameObject.h"

// Inherits from GameObject
class BulletObject : public GameObject {
public:
	BulletObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements, glm::vec3 target);

	// Update function for moving the player object around
	void update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[4], int size, glm::vec3 target) override;
	inline void setTimeShot(time_t newTime) { timeShot = newTime; }
	float shotExpired();
	void deleteBullet();

private:
	float distance = 0;	//Used for deleting later
	float bulletSpeed = 4.0f;
	float bulletRotation;

	float airTime = 5.0f;
	time_t timeShot;

	glm::vec3 aimTarget;
	glm::vec3 calcVel;
};