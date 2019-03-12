#pragma once

#include "math.h"
#include "GameObject.h"
#include "BulletObject.h"
#include "TargetObject.h"

// Inherits from GameObject
class PlayerGameObject : public GameObject {
public:
	PlayerGameObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	void update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[4], int size, glm::vec3 target) override;

	float shotExpired();	//Just for virtual purposes
	void deleteBullet();	

	inline float getSpeed() override { return playerSpeed; }
	inline void resetMoveTime() { moveCount = 0; }

	inline void addTime(float time) { moveCount += time; }
	inline float getTime() { return moveCount; }

	glm::vec3 getNearest(std::vector<GameObject*> gameObjects);
	inline void flipState() { if (state == "hunt") state = "flee"; else state = "hunt"; }
	inline void setState(string str) { state = str; }
	inline string getState() { return state; }

private:
	float playerSpeed = 0.1f;
	float maxSpeed = 0.1f;
	float accelerationMax = 0.5f;

	float playerRotationSpeed = 300.0f;

	int maxBullets = 10;
	int bulletCount = 0;
	float coolDown = 0.25;
	time_t lastShot;
	float moveCount = 1.0f;

	//Finite State Functions and pathing
	string state = "hunt";
	glm::vec3 lastRamPos;

	void stateHunt(glm::vec3 targetPos, double deltaTime);	//Using chase behaivour
	void stateFlee(glm::vec3 targetPos, double deltaTime);	//Using diff behaviour
	void stateSnipe(glm::vec3 targetPos, double deltaTime, std::vector<GameObject*> &gmObjects, GLuint bulletTex, int size);	//Takes a shot at the nearest target
};
