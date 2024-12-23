#version 460 core

in vec3 ourColor;
in vec3 vertexPosition;

out vec4 FragColor;

uniform bool useVertexAsColor = true;

void main()
{
	FragColor = useVertexAsColor ?
		vec4(vertexPosition, 1.0) : vec4(ourColor, 1.0);	// Exercise 3
}
