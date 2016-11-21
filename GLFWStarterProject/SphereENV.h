#pragma once
#include "Geode.h"
#include <vector>
#include "../Window.h"

class SphereENV :
	public Geode
{
public:
	SphereENV();
	~SphereENV();

	void solidSphere(GLuint shaderProgram, glm::vec3 camera_pos, GLuint skyboxTexture);
	void draw(GLuint shaderProgram);
	void render();
	void update();

	// These variables are needed for the shader program
	GLuint VBO, VAO, NBO;
	GLuint uProjection, uModelview, uView, uModel;

	GLuint segments = 50;
	GLfloat radius = 1.0f;
	int stacks = 10;
	int slices = 10;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> sphereVertices;
	std::vector<glm::vec3> sphereNormals;

	glm::mat4 toWorld = glm::mat4(1.0f);

	GLuint textureID;

};

