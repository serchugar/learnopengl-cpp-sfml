//#include <glad/glad.h>
#include <SFML/Window.hpp>
//#include "createWindow.h"
#include <iostream>

void createWindow()
{
    // Set up OpenGL context settings
    sf::ContextSettings settings;
    settings.majorVersion = 4;                           // OpenGL context version (major)
    settings.minorVersion = 6;                           // OpenGL context version (minor)
    settings.attributeFlags = sf::ContextSettings::Core; // Use OpenGL Core Profile
    //settings.depthBits = 24;                             // Depth buffer precision
    //settings.stencilBits = 8;                            // Stencil buffer precision

    // Create the window with the OpenGL context
    sf::Window window(sf::VideoMode(800, 600), "SFML OpenGL Context",
        sf::Style::Default, settings);

    // Print out the actual context settings. The CMD will only appear if it is a console app (main). Won't appear if it is Windows app (WinMain)
    sf::ContextSettings actual = window.getSettings();
    std::cout << "OpenGL Context: " << actual.majorVersion << "." << actual.minorVersion << std::endl;

    // Keep the window open until manually closed
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}