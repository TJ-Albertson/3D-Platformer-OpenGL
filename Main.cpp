//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"


const unsigned int width = 1280;
const unsigned int height = 720;


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "Game"
	GLFWwindow* window = glfwCreateWindow(width, height, "Game", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(0.8f, 0.8f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(1.5f, 0.8f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);





	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);


	bool mouseLock = false;
	// Creates camera object


	//glm::make_vec3 cameraPointer = &cameraPosition;

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 5.0f), mouseLock);


	// Original code from the tutorial
	Model cubeModel("models/cube/scene.gltf");
	Model playerModel("models/player/scene.gltf");



	glm::vec3 playerLocation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cubeLocation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat playerRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);


	float yaw = 0.0f;
	float pitch = 0.0f;
	float radius = 6.0f;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.843757f, 0.90625f, 0.97265625f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f, cubeLocation, yaw, radius, pitch);

		// Draw a model
		cubeModel.Draw(shaderProgram, camera, cubeLocation, rotation);
		playerModel.Draw(shaderProgram, camera, playerLocation, playerRotation);

		std::cout << pitch << std::endl;


		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			yaw+=0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			yaw-=0.01f;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			if (pitch < 1) {
				pitch += 0.01f;
				radius -= 0.01f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			if (pitch > -0.33) {
				pitch -= 0.01f;
				radius += 0.01f;
			}			
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cubeLocation += glm::vec3(0.1f, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cubeLocation += glm::vec3(0.0f, 0.0f, 0.1f);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cubeLocation += glm::vec3(-0.1f, 0.0f, 0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cubeLocation += glm::vec3(0.0f, 0.0f, -0.1f);
		}


		
		//camera.Position = playerLocation + glm::vec3(0.0f, 1.0f, 0.0f);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}