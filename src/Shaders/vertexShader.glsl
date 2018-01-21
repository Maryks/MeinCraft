#version 120

attribute vec3 vertexCoordinates;
attribute vec2 textureCoordinates;

uniform mat4 projectionViewMatrix;

varying vec2 passTexCoordinates;

void main()
{
	gl_Position = projectionViewMatrix * vec4(vertexCoordinates.x, vertexCoordinates.y, vertexCoordinates.z, 1.0);

	passTexCoordinates = textureCoordinates;
}