#pragma once
#include "Includes.h"
#include "TextureLoader.h"

class Shape {
public:
	// variables
	GLuint vao, vbo, ibo;
	GLuint normalVBO, colorVBO, texCordVBO;
	TextureLoader texture = TextureLoader();

	// functions
	void initShape();
	void setTexture(const char*);
	void render();
};

class Torus : public Shape{
public:
	// Variables
	int numVertices = 10;
	int numTorusSlices = 10;
	float torusRadius = 1.0f;
	float tubeRadius = 0.3f;

	// Functions
	void initShape();
	void render();
};