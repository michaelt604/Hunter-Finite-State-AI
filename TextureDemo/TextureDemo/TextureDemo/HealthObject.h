#pragma once
#include "GameObject.h"

class HealthObject : public GameObject {
public:
	HealthObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements);
	
	// Update function for moving the player object around
	void update(double deltaTime, std::vector<GameObject*> &gmObjects, GLuint textures[4], int size, glm::vec3 target) override;

	void render(Shader &shader, std::vector<GameObject*> &gmObjects, double deltaTime) override;
};

