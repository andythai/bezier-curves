#pragma once
#include "Geode.h"
#include <vector>
#include "../Window.h"

class Sphere :
	public Geode
{
public:
	Sphere(int sid);
	~Sphere();

	void solidSphere(GLuint shaderProgram, bool green);
	void draw(GLuint shaderProgram);
	void render();
	void update();
	void selection_buffer(GLuint shaderProgram);

	// These variables are needed for the shader program
	GLuint VBO, VAO, NBO;
	GLuint uProjection, uModelview, uView, uModel, idLocation;

	int id;
	GLuint segments = 50;
	GLfloat radius = 0.1f;
	int stacks = 10;
	int slices = 10;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> sphereVertices;
	std::vector<glm::vec3> sphereNormals;

	glm::mat4 toWorld = glm::mat4(1.0f);
};

