#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/mat4x3.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include "../Window.h"

class Bezier
{
public:
	Bezier();
	~Bezier();
	void draw(GLuint shaderProgram);
	void drawLines(GLuint shaderProgram);
	void lerp(int start);
	void relerp();
	int highest();

	GLuint VBO, VAO;
	GLuint uProjection, uModel, uModelview, uView;
	std::vector<glm::vec3> control = std::vector<glm::vec3>();
	std::vector<glm::vec3> points = std::vector<glm::vec3>();
	std::vector<glm::vec3> lines = std::vector<glm::vec3>();

	int highest_point_index = 0;

	const glm::mat4 B_bez = { -1, 3, -3, 1,
							  3, -6, 3, 0,
							 -3, 3, 0, 0,
							  1, 0, 0, 0 };

	glm::mat4 toWorld = glm::mat4(1.0f);
};

