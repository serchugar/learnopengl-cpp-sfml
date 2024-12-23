#include "helloTriangle.hpp"
#include <SFML/Window.hpp>
#include <glad/glad.h>
#include <iostream>

// Function prototypes
bool processInput(sf::Window& window, bool& toffleWireframe);
void checkCompilation(unsigned int shader, std::string errorMsg);
void checkLinking(unsigned int shaderProgram, std::string errorMsg);

// Settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// Source code of shaders
const char* vertexShaderSourceEx3_2 = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSourceEx3_2 = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"							// Orange
"}\0";

const char* fragmentShaderSourceEx3_3 = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(255.0f/255.0f, 212.0f/255.0f, 13.0f/255.0f, 1.0f);\n"	// Goldish Yellow
"}\0";

// Main program
void exercise3_3()
{
	// Set up SFML OpenGL context settings
	sf::ContextSettings gLsettings;
	gLsettings.majorVersion = 4;							// OpenGL context version (major)
	gLsettings.minorVersion = 6;							// OpenGL context version (minor)
	gLsettings.attributeFlags = sf::ContextSettings::Core;	// Use OpenGL Core Profile

	// Create the window with the SFML/OpenGL context
	sf::Window window;
	window.create(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "LearnOpenGL", sf::Style::Default, gLsettings);

	// GLAD initialization BEFORE we run any GL function. So that we can use all the GL functions
	if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		std::exit(EXIT_FAILURE); // Exit with failure code
	}

	// SHADER COMPILATION
	// ----------------------------------------------------------------------------------------------------------------
	unsigned int vertexShader, fragmentShader, fragmentShaderYellow;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourceEx3_2, NULL);
	glCompileShader(vertexShader);
	checkCompilation(vertexShader, "ERROR::SHADER::VERTEX::COMPILATION_FAILED");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceEx3_2, NULL);
	glCompileShader(fragmentShader);
	checkCompilation(fragmentShader, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");

	fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceEx3_3, NULL);
	glCompileShader(fragmentShaderYellow);
	checkCompilation(fragmentShaderYellow, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
	// ----------------------------------------------------------------------------------------------------------------

	// SHADER PROGRAM 
	// ----------------------------------------------------------------------------------------------------------------
	unsigned int shaderProgram, shaderProgramYellow;

	// Basic orange shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkLinking(shaderProgram, "ERROR::SHADER::PROGRAM::LINKING_FAILED");

	// Basic yellow shader program
	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);
	checkLinking(shaderProgramYellow, "ERROR::SHADER::PROGRAM::LINKING_FAILED");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShaderYellow);
	// ----------------------------------------------------------------------------------------------------------------

	// VERTICES TO BE DRAWN
	// ----------------------------------------------------------------------------------------------------------------
	// Triangle 1
	float vertices1[] =
	{
		-0.5f,  0.5f, 0.0f, // Top Left.        3
		 0.5f, -0.5f, 0.0f, // Bottom Right.    1
		-0.5f, -0.5f, 0.0f, // Bottom Left.     0
	};
	// Triangle 2
	float vertices2[] =
	{
		-0.5f,  0.5f, 0.0f, // Top Left.        3
		 0.5f,  0.5f, 0.0f, // Top Right.       4
		 0.5f, -0.5f, 0.0f, // Bottom Right.    1
	};
	// ----------------------------------------------------------------------------------------------------------------

	// VAO, VBO
	// ----------------------------------------------------------------------------------------------------------------
	unsigned int VAO1, VAO2, VBO1, VBO2;

	// Triangle 1
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Vertex linking attributes
	glEnableVertexAttribArray(0);

	// Triangle 2
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Vertex linking attributes
	glEnableVertexAttribArray(0);
	// ----------------------------------------------------------------------------------------------------------------

	// RENDER LOOP
	// ----------------------------------------------------------------------------------------------------------------
	bool running = true;
	bool toggleWireframe = false;
	while (running)
	{
		running = processInput(window, toggleWireframe);

		// Clear the buffers with dark grey color
		glClearColor(60.0f/255.0f, 60.0f/255.0f, 60.0f/255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the two triangles
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

        // End the current frame (internally swaps the front and back buffers)
		window.display();
	}
	// ----------------------------------------------------------------------------------------------------------------

	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteProgram(shaderProgram);
}


bool processInput(sf::Window& window, bool& toggleWireframe)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			return false;

		else if (event.key.code == sf::Keyboard::Escape)
			return false;

		else if (event.type == sf::Event::Resized)
			glViewport(0, 0, event.size.width, event.size.height);

        else if (event.type == event.KeyPressed && event.key.code == sf::Keyboard::W)
        {
			toggleWireframe = !toggleWireframe;
			toggleWireframe ?
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
	}
	return true;
}

void checkCompilation(unsigned int shader, std::string errorMsg)
{
    int success; // Integer to indicate success
    char infoLog[512]; // Storage container for the error messages if any
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Used to check if compilation was successful
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << errorMsg << "\n" << infoLog << std::endl;
    }
}

void checkLinking(unsigned int shaderProgram, std::string errorMsg)
{
    int success; // Integer to indicate success
    char infoLog[512]; // Storage container for the error messages if any
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // Used to check if compilation was successful
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << errorMsg << "\n" << infoLog << std::endl;
    }
}
