//copied from <common/contro;.cpp
// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include <stdio.h>

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix2(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix2(){
	return ProjectionMatrix;
}

// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14/2.0f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 55.0f;
float radius = 5.0f;
glm::vec3 posCam = glm::vec3( 0, 0, 5 ); 

float mouseSpeed = 0.005f;

glm::vec3 posLookAt = glm::vec3( 0, 0, 0 );

void computeMatricesFromInputs2(){
	static int inited = 0;
	if(inited == 0) {//Jammy fixed bug
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL );
	glfwSetCursorPos(window, 1024/2, 768/2);
	inited = 1;
	}
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	static double xold= 512, yold=384;
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	// Reset mouse position for next frame
//	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(xpos-xold);//1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( ypos - yold);//768/2 - ypos );
	xold = xpos; yold = ypos;
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);


	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		radius += 0.1;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		radius -= 0.1;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		posLookAt.x += 0.1;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		posLookAt.x -= 0.1;
	}
	posCam = radius*direction; 
printf("pos= %f,%f,%f\n", posCam.x, posCam.y, posCam.z);
	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		posCam,           // Camera is here
		posLookAt,//
		glm::vec3(0,1,0)//up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
