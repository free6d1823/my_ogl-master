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

enum ViewMode {
    VM_NONE = 0,
    VM_BIRDVIEW = 1,    //see from top
    VM_BEHINE = 2,      //see behind car
    VM_AHEAD = 3,       //see in front of car
    VM_FRONT = 4,       //sit inside car see front
    VM_BACK = 5,        // see back
    VM_LEFT = 6,        // see left side
    VM_RIGHT = 7,       //see right side
};
ViewMode m_eCurrentMode = VM_BIRDVIEW;
ViewMode m_eLastMode = VM_NONE;

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
////////////////////////////////////////////
glm::vec3 posCam = glm::vec3( 0, 0, 5 ); 
glm::vec3 posLookAt = glm::vec3( 0, 0, 0 );
glm::vec3 camUp = glm::vec3( 0, 1, 0 );
float m_FoV = 55;
///////////////////////////////////////////

float mouseSpeed = 0.005f;


mat4 ObjectMatrix = mat4(0);
mat4 getObjectMoveMat()
{
    return ObjectMatrix;
}
/* process key definition VM_BIRDVIEW */
void doBirdView() {
    if(m_eLastMode != m_eCurrentMode) {
        //init this mode
        m_eLastMode = m_eCurrentMode;
        m_FoV = 60;
        posCam.x = 5;
        posCam.y = 0;
        posCam.z = 0;
        posLookAt.x = 0;
        posLookAt.y = 0;
        posLookAt.z = 0;
        camUp = vec3( 0, 1, 0 );
    }
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		posCam.z -= 0.1;
	}
	// Move backward
	else if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		posCam.z += 0.1;
	}
	// Strafe right
    float rotateSpeed = 0.02;
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		camUp = camUp * mat3(cos(rotateSpeed), -sin(rotateSpeed), 0, sin(rotateSpeed), cos(rotateSpeed), 0, 0,0,1);
	}
	// Strafe left
	else if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		camUp = camUp * mat3(cos(rotateSpeed), sin(rotateSpeed), 0, -sin(rotateSpeed), cos(rotateSpeed), 0, 0,0,1);
	}
}
void computeMatricesFromInputs2(){
	static int inited = 0;
	if(inited == 0) {//Jammy fixed bug
	    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL );
	    glfwSetCursorPos(window, 1024/2, 768/2);
	    inited = 1;
	}
    //check view mode
	if (glfwGetKey( window, GLFW_KEY_K ) == GLFW_PRESS){
		m_eCurrentMode = VM_BIRDVIEW;
	} else if(glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS) {
		m_eCurrentMode = VM_BEHINE;
	} else if(glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS) {
		m_eCurrentMode = VM_AHEAD;
	} else if(glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS) {
		m_eCurrentMode = VM_FRONT;
	} else if(glfwGetKey( window, GLFW_KEY_M ) == GLFW_PRESS) {
		m_eCurrentMode = VM_BACK;
	} else if(glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS) {
		m_eCurrentMode = VM_LEFT;
	} else if(glfwGetKey( window, GLFW_KEY_J ) == GLFW_PRESS) {
		m_eCurrentMode = VM_RIGHT;
    }

    switch(m_eCurrentMode) {
        case VM_BIRDVIEW:
            doBirdView(); break;
    }
#if 0
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
//printf("pos= %f,%f,%f\n", posCam.x, posCam.y, posCam.z);
	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
#endif

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(m_FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		posCam,           // Camera is here
		posLookAt,//
		camUp//up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

    static float fTx = 0;
    static float fRy = 0;

    ObjectMatrix = mat4(0);
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		fRy -= 0.01;
	}
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
        fRy += 0.01;
	}
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
            //car forward
        fTx -= 0.01;
    }
    if (glfwGetKey( window, GLFW_KEY_X ) == GLFW_PRESS){
        fTx += 0.01;
    }
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
        fTx = 0; fRy = 0;
    }
    mat4 tx = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,fTx,1);
    ObjectMatrix = mat4(cos(fRy),0,sin(fRy), 0,
                        0,       1,       0, 0,
                        -sin(fRy),0,cos(fRy), 0, 
                        0,0,0,1)* tx;
	// For the next frame, the "last time" will be "now"
	//lastTime = currentTime;
}
