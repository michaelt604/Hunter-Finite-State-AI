#include "GameObject.h"

#pragma once
class ExplosionObject : public GameObject {
public:
	ExplosionObject(glm::vec3 entityPos, float entityRotation, glm::vec3 entityScale, GLuint entityTexture, GLint entityNumElements);

	// Renders the GameObject using a shader
	void render(Shader &shader, std::vector<GameObject*> &gmObjects, double deltaTime);

};

