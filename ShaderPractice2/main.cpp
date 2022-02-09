#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

const GLfloat PI = 3.14159265358979323846;


//Window dimensions
const GLint WIDTH = 1500, HEIGHT = 1125;

GLuint VAO, VBO, EBO, shader;

//Vertex Shader
static const char* vertexShaderSource = "		\n\
#version 330 core								\n\
												\n\
layout (location = 0) in vec3 pos;				\n\
layout (location = 1) in vec3 col;				\n\
												\n\
out vec3 ourColor;								\n\
												\n\
void main(){									\n\
	gl_Position = vec4( pos, 1.0);				\n\
	ourColor = col;								\n\
}";

//Fragment Shader
static const char* fragmentShaderSource = "		\n\
#version 330 core								\n\
												\n\
out vec4 FragColor;								\n\
in vec3 ourColor;								\n\
												\n\
void main(){									\n\
	FragColor = vec4(ourColor, 1.0);			\n\
}";

//Function prototypes
void processInput(GLFWwindow* window);
void CompileShaders();
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
void CreateTriangle(GLfloat cX, GLfloat cY, GLfloat radius);
void renderTriangle();
void CreateRectangle();
void RenderRectangle();
void CreateHouse();
void RenderHouse();

int main() {
	//Initialize GLFW window and Check if created
	if (!glfwInit()) {
		std::cout << "GLFW Initialization Failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Configure GLFW window properties Major and Minor
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Apple OS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//GLFW window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	//check if window is created
	if (!window) {
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Get window buffer width and height
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	//Set context
	glfwMakeContextCurrent(window);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	//GLEW Initialization
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW Initialization failed!" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	//Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Create triangle and Compile Shaders
	CreateTriangle(0.0f, 0.0f, 0.2f);
	//CreateRectangle();
	//CreateHouse();
	CompileShaders();

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		//Get inputs and process
		processInput(window);
		glfwPollEvents();

		//Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderTriangle();
		//RenderRectangle();
		//RenderHouse();


		glfwSwapBuffers(window);
	}

	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void CompileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		std::cout << "Error Creating Shader Program!" << std::endl;
		return;
	}

	AddShader(shader, vertexShaderSource, GL_VERTEX_SHADER);
	AddShader(shader, fragmentShaderSource, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Linking Program!" << eLog << std::endl;
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program!" << eLog << std::endl;
		return;
	}
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {

	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the shader: " << eLog << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);

	return;
}

void CreateTriangle(GLfloat cX, GLfloat cY, GLfloat radius) {
	/*
	GLfloat vertices[] = {
		-0.2f, 0.2f, 0.0f,
		-0.2f, -0.2f, 0.0f,
		0.2f, -0.2f, 0.0f
	};
	*/
	GLfloat Fx, Fy, Rx, Ry, Lx, Ly;
	Fx = cX + 0.0f;
	Fy = cY + 0.5f;
	Rx = Fx * cos(-135 * (PI / 180)) - Fy * sin(-135 * (PI / 180));
	Ry = Fy * cos(-135 * (PI / 180)) + Fx * sin(-135 * (PI / 180));
	Lx = Fx * cos(135 * (PI / 180)) - Fy * sin(135 * (PI / 180));
	Ly = Fy * cos(135 * (PI / 180)) + Fx * sin(135 * (PI / 180));

	/*
	Rx = Fx * cos(-90 * (PI / 180)) - Fy * sin(-90 * (PI / 180));
	Ry = Fy * cos(-90 * (PI / 180)) + Fx * sin(-90 * (PI / 180));
	Lx = Fx * cos(90 * (PI / 180)) - Fy * sin(90 * (PI / 180));
	Ly = Fy * cos(90 * (PI / 180)) + Fx * sin(90 * (PI / 180));
	*/

	GLfloat vertices[] = {
		//positions		//colors
		Fx, Fy, 0.0f,	1.0f, 0.0f, 0.0f,
		Rx, Ry, 0.0f,	0.0f, 1.0f, 0.0f,
		Lx, Ly, 0.0f,	0.0f, 0.0f, 1.0f
	};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//Deallocate vao and vbo and reuse them

}

void renderTriangle() {
	//Program being used
	glUseProgram(shader);
	//Bind VAO
	glBindVertexArray(VAO);
	//Draw a shape
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//Unbind VAO
	glBindVertexArray(0);
	//Stop using program
	glUseProgram(0);
}

void CreateRectangle() {
	GLfloat vertices[] = {
		0.2f, 0.2f, 0.0f,	//top right
		0.2f, -0.2f, 0.0f,	//bottom right
		-0.2f, -0.2f, 0.0f,	//bottom left
		-0.2f, 0.2f, 0.0f	//top left
	};
	GLint indices[]{
		0, 1, 3,	//triangle 1
		1, 2, 3		//triangle 2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//deallocate vao and vbo and ebo
}

void RenderRectangle() {
	glUseProgram(shader);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void CreateHouse() {
	GLfloat vertices[] = {
		0.2f, 0.2f, 0.0f,	//top right
		0.2f, -0.2f, 0.0f,	//bottom right
		-0.2f, -0.2f, 0.0f,	//bottom left
		-0.2f, 0.2f, 0.0f,	//top left
		0.0f, 0.4f, 0.0f	//very top
	};
	GLint indices[]{
		0, 1, 3,	//triangle 1
		1, 2, 3,	//triangle 2
		0, 3, 4		//triangle 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//deallocate vao and vbo and ebo
}

void RenderHouse() {
	glUseProgram(shader);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}