#include "shaders.hpp"
#include "shaderClass.hpp"

#include <SFML/Window.hpp>
#include <glad/glad.h>
#include <iostream>

// Function prototypes
bool processInput(sf::Window& window);

// Settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

void shaders()
{
	// SFML OpenGL Settings
	sf::ContextSettings settings;
	settings.majorVersion = 4;
	settings.minorVersion = 6;
	settings.attributeFlags = sf::ContextSettings::Core;

	// SFML Window
	sf::Window window;
	window.create(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "Shaders", sf::Style::Default, settings);

	// Set max framerate to 160
	window.setFramerateLimit(160);

	// GLAD Initialization
	if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		std::exit(EXIT_FAILURE); // Exit with failure code
	}

	// Check of many vertex attributes (inputs of the vertex shader) are available to use
	// My hardware allows for 32 inputs
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// Shaders managed by our Shader class
	Shader shader1("4-Shaders/vertexShader.vert", "4-Shaders/fragmentShader.frag");

	// Define vertex geometry of the triangle
	float vertices[] =
	{
		// Positions		// Colors
		0.0f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// Top
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	// Bottom Right
	   -0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	// Bottom Left
	};

	// Set up buffers and configure vertex attributes
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// Render loop
	bool running = true;
	sf::Clock clock;
	while (running)
	{
		running = processInput(window);

		// Clear screen with dark grey color
		glClearColor(60.0f/255.0f, 60.0f/255.0f, 60.0f/255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader
		shader1.use();

		// Update the uniforms
		float timevalue = clock.getElapsedTime().asSeconds();
		shader1.setFloat("xOffset", timevalue/5);
		shader1.setBool("invertYAxis", true);

		// This one makes the bottom left black because we are using the vertex data as
		// the rgb value. The vertices coordinates are in Normalized Device Coordinates
		// NDC, where (0,0) is the center of the screen. RGB values in OpenGL range from
		// 0-1 instead of 0-255. Thus, whatever is left and below of the center of the
		// screen will fade towards black. (-0.5,-0.5) coord is black because R=-0.5,
		// G=-0.5 and B=0 (since we are working with 2D shapes)
		shader1.setBool("useVertexAsColor", true);

		// Render the triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// End the current frame (internally swaps front and back buffers)
		window.display();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

bool processInput(sf::Window& window)
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
	}
	return true;
}
