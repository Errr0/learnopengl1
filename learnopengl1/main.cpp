#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
//GL_STATIC_DRAW: the data is set only once and used many times.
//GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

void WireframeMode(bool on = 0) {
	if (on) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void glInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// on MAC OS this line is needed to work
}

////////////////////////////////////////////////////////////////////////////////
// ----------------------------------Window---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* createWindow() {
	GLFWwindow* window = glfwCreateWindow(800, 600, "Name", NULL, NULL);
	if (window == NULL) {
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return window;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoaderLoadGL()) {
		std::cout << "failed to initialize GLAD" << std::endl;
		return window;
	}

	glViewport(0, 0, 800, 600);//tells how big window is; first two arguments are bottom-left corner of window, third and fourth are width and hight

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------RenderLoopShit------------------------------- //
////////////////////////////////////////////////////////////////////////////////

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


////////////////////////////////////////////////////////////////////////////////
// ---------------------------------Shaders---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
"}\0";

unsigned int makeVertexShader() {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//debug shit
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	return vertexShader;
}

unsigned int makeFragmentShader(const char* fragmentShaderSource) {
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//debug shit
	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	return fragmentShader;
}

void destroyShaders(unsigned int& vertexShader, unsigned int& fragmentShader) {
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

unsigned int makeShaderProgram(unsigned int &vertexShader, unsigned int &fragmentShader) {
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//destroyShaders(vertexShader, fragmentShader);

	return shaderProgram;
}

////////////////////////////////////////////////////////////////////////////////
// ----------------------------------Main------------------------------------ //
////////////////////////////////////////////////////////////////////////////////

int main() {
	glInit();
	GLFWwindow* window = createWindow();
	if (window == NULL) return -1;
	
	float vertices[] = {
		-1.0f, -0.5f, 0.0f,
		 0.0f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	float vertices2[] = {
			 1.0f, -0.5f, 0.0f,
			 0.0f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f
	};
	
	unsigned int indices[] = {
		0, 1, 2 // first triangle
		//0, 1, 2, // first triangle
		//1, 2, 4 // second triangle
	};
	/*
	float vertices[] = {
		 0.5f,  0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f // top left
	};
	unsigned int indices[] = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};
	*/


	

	unsigned int vertexShader = makeVertexShader();
	
	unsigned int fragmentShader = makeFragmentShader(fragmentShaderSource);
	unsigned int fragmentShader2 = makeFragmentShader(fragmentShaderSource2);

	//linking shaders
	unsigned int shaderProgram = makeShaderProgram(vertexShader, fragmentShader);
	unsigned int shaderProgram2 = makeShaderProgram(vertexShader, fragmentShader2);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);
	//Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	//Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	////////////////////
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	WireframeMode();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//drawing
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//rest of shit
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
//str 42 chapter 6.0