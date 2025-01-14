#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader source code, used to transform shape within the window
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code, used to manipulate color, shader, and texture (color in this case)
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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

	//create vertices
	GLfloat vertices[] =
	{
		//Low left, low right, upper
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};



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

	//Create vertex shader and fragment shader, then compile the program into the vertexShader var
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//same as above but for fragment shader
	GLuint fragmentShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//generic shader program for vertex and frag shaders
	GLuint shaderProgram = glCreateProgram();

	//attach the programs generated onto the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//link all programs together
	glLinkProgram(shaderProgram);

	//clean-up shaders now that we are done with them.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//declare Vertex Array and Buffer Reference objects
	GLuint VAO, VBO;

	//generate both objects with 1 object (the triangle)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bring out array into the forefront by binding
	glBindVertexArray(VAO);

	//bind buffer, defining array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//add vertices into buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//configure vertex attribute, so OpenGL understands how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *sizeof(float), (void *)0);
	//Enable vertex attribute so that OpenGL uses it
	glEnableVertexAttribArray(0);


	//set buffer to 0, so we don't modify unintentionally
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//create new color for back buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//swap front to back to display color
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window))
	{
		//set color, use program and bind VAO
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//draw triangles, then swap buffers to display
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	//cleanup buffers and arrays
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);

	//stop glfw
	glfwTerminate();
	return 0;
}