#include "BulletObject.h"
#include "Window.h"

/*
	BulletObject inherits from GameObject
	It overrides GameObject's update method, so that it uses it's own update
*/

BulletObject::BulletObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements, glm::vec3 target)
			: GameObject(entityPos, entityRotation, entityScale, entityTexture, entityNumElements)
{
	bulletSpeed = 5.0f;
	aimTarget = target;
	calcVel = bulletSpeed * ((aimTarget - position) / glm::length(aimTarget - position));
}


// Update function for moving the bullet
void BulletObject::update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[4], int size, glm::vec3 target) {
	velocity = calcVel * (float)deltaTime;	//Calculates new velocity with dt
	
	// Call the parent's update method to move the object
	GameObject::update(deltaTime, gmObjects, textures, size, target);
}

float BulletObject::shotExpired()
{
	return (difftime(time(0), timeShot) > airTime);
}

void BulletObject::deleteBullet(){ }	//Here for virtualness

