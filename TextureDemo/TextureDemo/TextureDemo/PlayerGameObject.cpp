#include "PlayerGameObject.h"
#include <math.h>

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityRotation, entityScale, entityTexture, entityNumElements)
{
	lastShot = time(0);
}

// Update function for moving the player object around
void PlayerGameObject::update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[4], int size, glm::vec3 target) {
	// Checking for player input and changing velocity
	moveCount += deltaTime;
	if (state == "flee" && moveCount > 0.5f)	//Sets snipe state after flee cooldown
	{
		state = "snipe";
		moveCount = 0;
	}

	if (state == "hunt")	//If state is hunt
	{
		stateHunt(target, deltaTime);
	}

	else if (state == "flee")	//If state is flee
	{
		stateFlee(target, deltaTime);

	}

	else if (state == "snipe")	//If state is snipe
	{
		stateSnipe(target, deltaTime, gmObjects, textures[3], numElements);
		state = "hunt";
	}

	GameObject::update(deltaTime, gmObjects, textures, size, target);
}

float PlayerGameObject::shotExpired() { return 0; }	//just here for virtualness

void PlayerGameObject::deleteBullet() {
	bulletCount--;
}

void PlayerGameObject::stateHunt(glm::vec3 targetPos, double deltaTime)	//Using chase behaivour
{
	lastRamPos = targetPos;	//set a position to run from if a kill occurs

	//Calculate the new velocity with acceleration
	glm::vec3 v_t = targetPos - position;
	//acceleration = accelerationMax * (v_t - velocity) / (sqrt(pow(v_t.x - velocity.x, 2) + pow(v_t.y - velocity.y, 2) + pow(v_t.z - velocity.z, 2)));
	acceleration = accelerationMax * (v_t - velocity) / glm::length(v_t - velocity);
	velocity += acceleration * (float)deltaTime;
	float absV = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));
	if (absV > maxSpeed)
	{
		velocity = maxSpeed * velocity / absV;
	}
	rotation = atan2(velocity.y, velocity.x) * 180 / M_PI;
}

void PlayerGameObject::stateFlee(glm::vec3 targetPos, double deltaTime)	//Using flee behaviour
{
	//Calculate the new velocity with acceleration (***NOTE: Doesn't look like it works, however the player just moves really fast and the best way to get away from the last position is just straight, look at asteriods for more visable avoidance)
	glm::vec3 v_t = position - lastRamPos;
	acceleration = accelerationMax * (v_t - velocity) / (sqrt(pow(v_t.x - velocity.x, 2) + pow(v_t.y - velocity.y, 2) + pow(v_t.z - velocity.z, 2)));
	velocity += acceleration * (float)deltaTime;

	float absV = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));

	if (absV > maxSpeed)	//If speed is over our max speed 
	{
		velocity = maxSpeed * velocity / absV;
	}
	rotation = atan2(velocity.y, velocity.x) * 180 / M_PI;
}

void PlayerGameObject::stateSnipe(glm::vec3 targetPos, double deltaTime, std::vector<GameObject*> &gmObjects, GLuint bulletTex, int size)	//Shoots at nearest target
{
	bulletCount++;
	BulletObject* bullet = new BulletObject(position, rotation, glm::vec3(0.1f, 0.1f, 0.1f), bulletTex, size, targetPos);
	bullet->setTimeShot(time(0));
	bullet->setSize(0.1f);
	gmObjects.push_back(bullet);
	lastShot = time(0);
}

glm::vec3 PlayerGameObject::getNearest(std::vector<GameObject*> gameObjects)
{
	float nearestDistance = INT_MAX;	//Set int to be max to override by first element
	glm::vec3 nearestTarget;
	
	for (int i = 1; i < gameObjects.size(); i++)	//Loop through all objects
	{
		if (typeid(*gameObjects[i]).name() == typeid(TargetObject).name())	//If type is a target the compare distances
		{
			glm::vec3 pos = gameObjects[i]->getPosition();

			float distance = sqrt(pow(pos.x - position.x, 2) + pow(pos.y - position.y, 2) + pow(pos.z - position.z, 2));

			if (distance < nearestDistance)	//If we have a closer distance
			{
				nearestDistance = distance;
				nearestTarget = pos;
			}
		}
	}

	return nearestTarget;
}