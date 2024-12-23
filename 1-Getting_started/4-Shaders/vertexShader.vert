#version 460 core

layout (location = 0) in vec3 aPos;		// Attribute position 0
layout (location = 1) in vec3 aColor;	// Attribute position 1

out vec3 ourColor;	// Output color to fragment shader
out vec3 vertexPosition;

uniform float xOffset;
uniform bool invertYAxis = false;

void main()
{
	vertexPosition = vec3(	aPos.x + xOffset,					// Exercise 2
							aPos.y*(invertYAxis ? -1.0 : 1.0),	// Exercise 1
							aPos.z);

	gl_Position = vec4(vertexPosition, 1.0);

	ourColor = aColor;
}