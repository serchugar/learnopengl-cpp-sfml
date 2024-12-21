#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

void helloWindow()
{
	// Set up SFML OpenGL context settings
	sf::ContextSettings gLsettings;
	gLsettings.majorVersion = 4;							// OpenGL context version (major)
	gLsettings.minorVersion = 6;							// OpenGL context version (minor)
	gLsettings.attributeFlags = sf::ContextSettings::Core;	// Use OpenGL Core Profile

	// Create the window with the SFML/OpenGL context
	sf::Window window;
	window.create(sf::VideoMode(800, 600), "LearnOpenGL", sf::Style::Default, gLsettings);

    // Activate the window
    window.setActive(true);

    // Render loop
    bool running = true;
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
        }

        // glClearColor is a State-setting function.
        // It is changing the state of GL_COLOR_BUFFER_BIT
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        // Clear the buffers. This is a State-using function
        glClear(GL_COLOR_BUFFER_BIT);

        // End the current frame (internally swaps the front and back buffers)
        // One buffer draws each pixel, and when all are drawn, it swaps to the render buffer
        // So it will only show the img when its ready
        window.display();
    }
}
