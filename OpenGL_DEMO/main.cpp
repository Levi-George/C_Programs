#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

//Initial Testing for OpenGL

int main()
{
	//init glfw
	glfwInit();

	//tell glfw what version we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//which profile to use, compatible vs core, old and new vs just new.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a basic window
	GLFWwindow* window = glfwCreateWindow(800, 800, "DEMO", NULL, NULL);

	//error checking 
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//bring window to current context
	glfwMakeContextCurrent(window);

	//load glad to configure OpenGL
	gladLoadGL();

	//define viewport, 0,0 to 800,800
	glViewport(0, 0, 800, 800);

	//create new color for back buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//swap front to back to display color
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}


	glfwDestroyWindow(window);

	//stop glfw
	glfwTerminate();
	return 0;
}