#include "helloTriangle.hpp"
#include <SFML/Window.hpp>
#include <glad/glad.h>
#include <iostream>

// https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp

// Source code of the basic vertex shader
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Source code of the basic fragment shader
const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

void helloTriangle()
{

	// Set up SFML OpenGL context settings
	sf::ContextSettings gLsettings;
	gLsettings.majorVersion = 4;							// OpenGL context version (major)
	gLsettings.minorVersion = 6;							// OpenGL context version (minor)
	gLsettings.attributeFlags = sf::ContextSettings::Core;	// Use OpenGL Core Profile

	// Create the window with the SFML/OpenGL context
	sf::Window window;
	window.create(sf::VideoMode(800, 600), "LearnOpenGL", sf::Style::Default, gLsettings);

    // Activate the window. Optional since we only have 1 window
    //window.setActive(true);

    // GLAD initialization BEFORE we run any GL function. So that we can use all the GL functions
    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) 
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE); // Exit with failure code
    }

    // Define vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom Left.     0
         0.5f, -0.5f, 0.0f, // Bottom Right.    1

         0.0f,  0.5f, 0.0f, // Top Middle.      2

        -0.5f,  0.5f, 0.0f, // Top Left.        3
         0.5f,  0.5f, 0.0f  // Top Right.       4
    };
    unsigned int indices[]
    {
        4, 1, 3,    // First triangle
        3, 1, 0     // Second triangle
    };

    // VERTEX ARRAY OBJECT VAO. Needed when using OpenGL Core profile
    // This step is shown towards the end of this chapter, but it needs to go before the VBO
    // ----------------------------------------------------------------------------------------------------------------
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Bind the Vertex Array Object VAO first, then the Vertex Buffer Object VBO, and then configure vertex attribs
    glBindVertexArray(VAO);
    // ----------------------------------------------------------------------------------------------------------------

    // VERTEX SHADER
    // ----------------------------------------------------------------------------------------------------------------
    // Define vertex buffer object VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO); // It doesn't detect glGenBuffers. After including <glad.glad.h> it works!
    
    // Bind the newly created buffer to the GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // GL_ARRAY_BUFFER now point to VBO. glBufferData copies the previously defined vertex data into the buffers memory.
    // So here we are copying "vertices" into VBO, which is the currently bound with GL_ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ----------------------------------------------------------------------------------------------------------------

    // COMPILING A SHADER
    // ----------------------------------------------------------------------------------------------------------------
    // Creation of shader object referenced by an ID
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // GL_VERETEX_SHADER is the type of shader we want to create

    // Atach the shader source code to the shader object and compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if compilation of shader was successful
    int success; // Integer to indicate success
    char infoLog[512]; // Storage container for the error messages if any
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Used to check if compilation was successful
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // ----------------------------------------------------------------------------------------------------------------

    // FRAGMENT SHADER
    // ----------------------------------------------------------------------------------------------------------------
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check if compilation of shader was successful
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // Used to check if compilation was successful
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // ----------------------------------------------------------------------------------------------------------------

    // SHADER PROGRAM: Is the final linked version of multiple shaders combined. We activate this when rendering objects
    // When linking shaders into a programs it needs to be in the same order as the graphics pipeline. The output of one
    // shader is used as input in the next.
    // ----------------------------------------------------------------------------------------------------------------
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram(); // Creates shader program object and return its ID reference
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if linking was succesful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // The result is a program object that we can activate using glUseProgram with the newly create 'shaderProgram' as
    // as its argument
    //glUseProgram(shaderProgram); // We will only use this when drawing something, within the render loop

    // We delete the shaders since we don't need them now that they are linked in the shader program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // ----------------------------------------------------------------------------------------------------------------

    // LINKING VERTEX ATTRIBUTES
    // ----------------------------------------------------------------------------------------------------------------
    // Here we specify how OpenGL should interpret out vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Now we need to enable the vertex attribute
    glEnableVertexAttribArray(0);
    // ----------------------------------------------------------------------------------------------------------------

    // ENTITY BUFFER OBJECT EBO. This buffer stores the indices of each vertex to draw
    // ----------------------------------------------------------------------------------------------------------------
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // ----------------------------------------------------------------------------------------------------------------

    // Render loop
    bool running = true;
    bool wireframeMode = false;
    wireframeMode ?
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (running)
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // Adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == event.KeyPressed && event.key.code == sf::Keyboard::W)
            {
                wireframeMode = !wireframeMode;
                wireframeMode ?
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        // glClearColor is a State-setting function.
        // It is changing the state of GL_COLOR_BUFFER_BIT
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        // Clear the buffers. This is a State-using function
        glClear(GL_COLOR_BUFFER_BIT);

        // DRAW THE TRIANGLE
        // ------------------------------------------------------------------------------
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);                     // Use this if no EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    // Use this if using EBO
        // ------------------------------------------------------------------------------

        // End the current frame (internally swaps the front and back buffers)
        // One buffer draws each pixel, and when all are drawn, it swaps to the render buffer
        // So it will only show the img when its ready
        window.display();
    }

    // Optional: De-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}
