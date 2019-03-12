#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>

#include "Shader.h"
#include "Window.h"
#include "GameObject.h"
#include "PlayerGameObject.h"
#include "TargetObject.h"
#include "BulletObject.h"
#include "HealthObject.h"
#include "ExplosionObject.h"

/***********************************************************************************************/
/*										Lengthy Comment

Name: Michael Ting
Student Number: 101068936
Explanation: (Not sure how much detail is required)
	1. Basic Start: Already have
	2. Euler Integration: Just reimplemented velocity after having removed it
	3. Hunting: Following the formula provided in class along with a sorting to find the closest asteriod and some
	   tweeking of the speeds, acceleration and velocity to give a very nice curved pathing
	4. Flee: Inversed the hunting behaviour to have the ship run away, removed collision with targets in this state and
	   added some code to flip the states appropriately.
	5. Flee Distance: Based on a timer as it was cleaner than measuring the distance from the last point
	   and accomplishes the same thing (also ta said we could)
	6. Add State: Added the state "snipe". After fleeing, will shoot a bullet at the direction of the nearest enemy before
	   switching to the hunt stage for its next target.
	7. Bonus - Asteriod Flee: Added a flee to asteriods to run away from the position of the player using the same flee code the player had (Note, this is quite costly this lots and lots of asteriods... you might need to turn down number of asteriods present (Line 176))
Bugs:
	- 

Extra: Gave the player a rotation so it looked in the direction it was going and looked cool

*/
/***********************************************************************************************/

using namespace std;

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Transform Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Global texture info
GLuint tex[5]; 

// Global game object info
std::vector<GameObject*> gameObjects;


// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[]  = {
		//  square (two triangles)
		   //  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer (6 in this case)
	return sizeof(face) / sizeof(GLuint);
}


void setthisTexture(GLuint w, char *fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setallTexture(void)
{
//	tex = new GLuint[4];
	glGenTextures(4, tex);
	//setthisTexture(tex[0], "orb.png");
	setthisTexture(tex[0], "ship.png");
	setthisTexture(tex[1], "saw.png");
	setthisTexture(tex[2], "rock.png");
	setthisTexture(tex[3], "bullet.png");
	setthisTexture(tex[4], "spaceDust.png");

	glBindTexture(GL_TEXTURE_2D, tex[0]);
}


// Main function that builds and runs the game
int main(void){
	try {
		// Seed for generating random numbers with rand()
		srand((unsigned int)time(0));

		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);

		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Create geometry of the square
		int size = CreateSquare();

		// Set up shaders
		Shader shader("shader.vert", "shader.frag");

		// Set up the textures
		setallTexture();

		// Setup the player object (position, texture, vertex count)
		PlayerGameObject* ourPlayer = new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), 90, glm::vec3(0.2f, 0.2f, 0.2f), tex[0], size);
		gameObjects.push_back(ourPlayer);
		gameObjects[0]->setSize(0.2);

		srand(time(NULL));
		// Setup targets objects with random numbers
		for (int i = 0; i < 200; i++)	//Spawn 200 new targets
		{
			float tempSize = ((rand() % 8) + 4) * 0.1f;
			float tempLocX = (rand() % 100 - 50) * 0.5f;
			float tempLocY = (rand() % 100 - 50) * 0.5f;
			int tempRot = (rand() % 360);

			TargetObject* tempTarget = new TargetObject(glm::vec3(tempLocX, tempLocY, 0.0f), tempRot, glm::vec3(tempSize, tempSize, tempSize), tex[rand() % 2] + 1, size);
			tempTarget->setSize(tempSize);

			gameObjects.push_back(tempTarget);
		}
		
		//Declare some extra variables
		float cameraZoom = 0.5f;	//Camera zoom out of main loop
		int shieldCount = 3;
		int score = 0;

		// Run the main loop
		double lastTime = glfwGetTime();
		while (!glfwWindowShouldClose(window.getWindow())) {
			// Clear background
			window.clear(viewport_background_color_g);

			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// Select proper shader program to use
			shader.enable();

			//Deals with scaling the view
			if (glfwGetKey(Window::getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {	//Zooms camera view in
				if (cameraZoom < 1.0f) cameraZoom += 0.03;
			}
			if (glfwGetKey(Window::getWindow(), GLFW_KEY_E) == GLFW_PRESS) {	//Zooms camera view out
				if (cameraZoom > 0.1f) cameraZoom -= 0.03f;
			}

			// Setup camera to focus on the player object (the first object in the gameObjects array)
			glm::vec3 cameraTranslatePos(-gameObjects[0]->getPosition());

			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			shader.setUniformMat4("viewMatrix", viewMatrix);

			// Update and render all game objects
			for (int i = 0; i < gameObjects.size(); i++) {
				// Get the current object
				GameObject* currentGameObject = gameObjects[i];

				// Updates game objects
				if (typeid(*currentGameObject).name() == typeid(TargetObject).name())
					currentGameObject->addRotation((float)deltaTime * 100.0f);

				if (typeid(*currentGameObject).name() == typeid(PlayerGameObject).name())	//Find correct state 
				{
					currentGameObject->update(deltaTime, gameObjects, tex, size, ourPlayer->getNearest(gameObjects));	//Updates the current game object
				}
				else
					currentGameObject->update(deltaTime, gameObjects, tex, size, ourPlayer->getPosition());	//Updates the current game object
				
					
				// Check for collision between game objects
				for (int j = i + 1; j < gameObjects.size(); j++) {
					GameObject* otherGameObject = gameObjects[j];

					if (typeid(*currentGameObject).name() == typeid(ExplosionObject).name())
					{
						int frames = currentGameObject->getFrameCount() - 1;
						if (frames == 0)
							gameObjects.erase(gameObjects.begin() + i);
						else
							currentGameObject->setFrameCount(frames);
						continue;
					}

					//If a collision occurs
					if (glm::length(currentGameObject->getPosition() - otherGameObject->getPosition()) < currentGameObject->getSize() * 0.5 + otherGameObject->getSize() * 0.5)	//If distance between two objects < obj1.size/2 + obj2.size/2
					{
						if (typeid(*currentGameObject).name() == typeid(TargetObject).name() && typeid(*otherGameObject).name() == typeid(BulletObject).name())	//Runs if bullet hits target
						{
							float scoreMod = (1 - currentGameObject->getSize()) * 1000;
							score += scoreMod;	//Increments score based on target size (smaller = more points)
							cout << "Score: " << score << endl;

							gameObjects.erase(gameObjects.begin() + j);	//Erases bullet
							gameObjects[0]->deleteBullet();				//Decrements bullet count
							gameObjects.push_back(new ExplosionObject(currentGameObject->getPosition(), 0, glm::vec3(currentGameObject->getSize(), currentGameObject->getSize(), currentGameObject->getSize()), tex[4], size));
							gameObjects.erase(gameObjects.begin() + i);	//Erases hit target
							
							//Create a new enemy (would make function for this but no real good place to put it
							float tempSize = ((rand() % 8) + 4) * 0.1f;
							float tempLocX = (rand() % 100 - 50) * 0.5f;
							float tempLocY = (rand() % 100 - 50) * 0.5f;
							int tempRot = (rand() % 360);

							TargetObject* tempTarget = new TargetObject(glm::vec3(tempLocX, tempLocY, 0.0f), tempRot, glm::vec3(tempSize, tempSize, tempSize), tex[rand() % 2] + 1, size);
							tempTarget->setSize(tempSize);

							gameObjects.push_back(tempTarget);
						}	
						
						if (typeid(*currentGameObject).name() == typeid(PlayerGameObject).name() && typeid(*otherGameObject).name() == typeid(TargetObject).name())	//Runs if player hits target
						{
							if (ourPlayer->getState() == "hunt")
							{
								ourPlayer->resetMoveTime();
								ourPlayer->setState("flee");

								float scoreMod = (1 - otherGameObject->getSize()) * 1000;
								score += scoreMod;	//Increments score based on target size (smaller = more points)
								cout << "Score: " << score << endl;
								gameObjects.erase(gameObjects.begin() + j);	//Erases hit target

								//Create a new enemy (would make function for this but no real good place to put it
								float tempSize = ((rand() % 8) + 4) * 0.1f;
								float tempLocX = (rand() % 100 - 50) * 0.5f;
								float tempLocY = (rand() % 100 - 50) * 0.5f;
								int tempRot = (rand() % 360);

								TargetObject* tempTarget = new TargetObject(glm::vec3(tempLocX, tempLocY, 0.0f), tempRot, glm::vec3(tempSize, tempSize, tempSize), tex[rand() % 2] + 1, size);
								tempTarget->setSize(tempSize);

								gameObjects.push_back(tempTarget);
							}
						}
					}
				}

				//Erase bullets after it expires
				if (typeid(*currentGameObject).name() == typeid(BulletObject).name() && currentGameObject->shotExpired())
				{
					cout << "Delete bullet" << endl;
					gameObjects.erase(gameObjects.begin() + i);	//Erases hit game object
					gameObjects[0]->deleteBullet();
				}


				// Render game objects
				currentGameObject->render(shader, gameObjects, deltaTime);
			}

			// Update other events like input handling
			glfwPollEvents();

			// Push buffer drawn in the background onto the display
			glfwSwapBuffers(window.getWindow());
		}
	}
	catch (std::exception &e){
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	return 0;
}
