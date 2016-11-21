#include "Bezier.h"

Bezier::Bezier()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	// First pair of curves
	glm::vec3 p1 = 2.0f * glm::vec3(-5.0f, 5.0f, 0.0f) - glm::vec3(-3.0f, 10.0f, 0.0f);
	glm::vec3 p2 = { 5.0f, 0.0f, 0.0f };
	glm::vec3 p0 = (glm::vec3(-3.0f, 10.0f, 0.0f) + p1) / 2.0f;

	glm::vec3 q1 = { 5.0f, 10.0f, 0.0f };
	glm::vec3 q2 = { 10.0f, 0.0f, 0.0f };
	glm::vec3 q3 = { 10.0f, 5.0f, 0.0f };
	
	glm::vec3 p3 = (p2 + q1) / 2.0f; // Dependent
	glm::vec3 q0 = glm::vec3(p3); // Dependent
	
	control.push_back(p0);
	control.push_back(p1);
	control.push_back(p2);
	control.push_back(p3);

	control.push_back(q0);
	control.push_back(q1);
	control.push_back(q2);
	control.push_back(q3);

	lerp(0);
	lerp(4);
	
	// Curve generation 3
	for (int j = 0; j < 1; j++) {
		glm::vec3 r0 = glm::vec3(control[j + 7]); // Dependent
		glm::vec3 r1 = glm::vec3(2.0f * r0 - control[j + 6]); // j
		float x2 = rand() % 10 - 5.0f;
		float y2 = rand() % 10 - 5.0f;
		float z2 = rand() % 10 - 5.0f;
		float x3 = rand() % 10 - 5.0f;
		float y3 = rand() % 10 - 5.0f;
		float z3 = rand() % 10 - 5.0f;
		glm::vec3 r2 = glm::vec3(x2, y2, z2);
		glm::vec3 r3 = glm::vec3(x3, y3, z3);
		control.push_back(r0);
		control.push_back(r1);
		control.push_back(r2);
		control.push_back(r3);
		lerp(8 + j); // j
	} 
	// 4
	for (int j = 0; j < 1; j++) {
		glm::vec3 r0 = glm::vec3(control[j + 11]); // Dependent
		glm::vec3 r1 = glm::vec3(2.0f * r0 - control[j + 10]); // j

		float x2 = rand() % 10 - 5.0f;
		float y2 = rand() % 10 - 5.0f;
		float z2 = rand() % 10 - 5.0f;

		float x3 = rand() % 10 - 5.0f;
		float y3 = rand() % 10 - 5.0f;
		float z3 = rand() % 10 - 5.0f;

		glm::vec3 r2 = glm::vec3(x2, y2, z2);
		glm::vec3 r3 = glm::vec3(x3, y3, z3);

		control.push_back(r0);
		control.push_back(r1);
		control.push_back(r2);
		control.push_back(r3);

		lerp(12 + j); // j
	}
	// 5
	for (int j = 0; j < 1; j++) {
		glm::vec3 r0 = glm::vec3(control[j + 15]); // Dependent
		glm::vec3 r1 = glm::vec3(2.0f * r0 - control[j + 14]); // j

		float x2 = rand() % 10 - 5.0f;
		float y2 = rand() % 10 - 5.0f;
		float z2 = rand() % 10 - 5.0f;

		float x3 = rand() % 10 - 5.0f;
		float y3 = rand() % 10 - 5.0f;
		float z3 = rand() % 10 - 5.0f;

		glm::vec3 r2 = glm::vec3(x2, y2, z2);
		glm::vec3 r3 = glm::vec3(x3, y3, z3);

		control.push_back(r0);
		control.push_back(r1);
		control.push_back(r2);
		control.push_back(r3);

		lerp(16 + j); // j
	}
	// 6
	for (int j = 0; j < 1; j++) {
		glm::vec3 r0 = glm::vec3(control[j + 19]); // Dependent
		glm::vec3 r1 = glm::vec3(2.0f * r0 - control[j + 18]); // j

		float x2 = rand() % 10 - 5.0f;
		float y2 = rand() % 10 - 5.0f;
		float z2 = rand() % 10 - 5.0f;

		float x3 = rand() % 10 - 5.0f;
		float y3 = rand() % 10 - 5.0f;
		float z3 = rand() % 10 - 5.0f;

		glm::vec3 r2 = glm::vec3(x2, y2, z2);
		glm::vec3 r3 = glm::vec3(x3, y3, z3);

		control.push_back(r0);
		control.push_back(r1);
		control.push_back(r2);
		control.push_back(r3);

		lerp(20 + j); // j
	}
	// 7
	for (int j = 0; j < 1; j++) {
		glm::vec3 r0 = glm::vec3(control[j + 23]); // Dependent
		glm::vec3 r1 = glm::vec3(2.0f * r0 - control[j + 22]); // j

		float x2 = rand() % 10 - 5.0f;
		float y2 = rand() % 10 - 5.0f;
		float z2 = rand() % 10 - 5.0f;

		float x3 = rand() % 10 - 5.0f;
		float y3 = rand() % 10 - 5.0f;
		float z3 = rand() % 10 - 5.0f;

		glm::vec3 r2 = glm::vec3(x2, y2, z2);
		glm::vec3 r3 = glm::vec3(x3, y3, z3);

		control.push_back(r0);
		control.push_back(r1);
		control.push_back(r2);
		control.push_back(r3);

		lerp(24 + j); // j
	}
	// 8
	for (int j = 0; j < 1; j++) {
		glm::vec3 r0 = glm::vec3(control[j + 27]); // Dependent
		glm::vec3 r1 = glm::vec3(2.0f * r0 - control[j + 26]); // j

		float x2 = rand() % 10 - 5.0f;
		float y2 = rand() % 10 - 5.0f;
		float z2 = rand() % 10 - 5.0f;

		float x3 = rand() % 10 - 5.0f;
		float y3 = rand() % 10 - 5.0f;
		float z3 = rand() % 10 - 5.0f;

		glm::vec3 r2 = glm::vec3(x2, y2, z2);
		glm::vec3 r3 = glm::vec3(x3, y3, z3);

		control.push_back(r0);
		control.push_back(r1);
		control.push_back(r2);
		control.push_back(r3);

		lerp(28 + j); // j
	}
	// 9
	for (int j = 0; j < 1; j++) {
		glm::vec3 r0 = glm::vec3(control[j + 31]); // Dependent
		glm::vec3 r1 = glm::vec3(2.0f * r0 - control[j + 30]); // j

		float x2 = rand() % 10 - 5.0f;
		float y2 = rand() % 10 - 5.0f;
		float z2 = rand() % 10 - 5.0f;

		float x3 = rand() % 10 - 5.0f;
		float y3 = rand() % 10 - 5.0f;
		float z3 = rand() % 10 - 5.0f;

		glm::vec3 r2 = glm::vec3(x2, y2, z2);
		glm::vec3 r3 = glm::vec3(-1.0f, 8.0f, 0);

		control.push_back(r0);
		control.push_back(r1);
		control.push_back(r2);
		control.push_back(r3);

		lerp(32 + j); // j
	}
	// 10
	
	for (int j = 0; j < 1; j++) {
		glm::vec3 r0 = glm::vec3(control[j + 35]); // Dependent
		glm::vec3 r1 = glm::vec3(2.0f * r0 - control[j + 34]); // j

		float x2 = rand() % 10 - 5.0f;
		float y2 = rand() % 10 - 5.0f;
		float z2 = rand() % 10 - 5.0f;

		glm::vec3 r2 = glm::vec3(-3.0f, 10.0f, 0.0f);
		glm::vec3 r3 = glm::vec3(-5.0f, 5.0f, 0.0f);


		control.push_back(r0);
		control.push_back(r1);
		control.push_back(r2);
		control.push_back(r3);

		lerp(36 + j); // j
	}

	highest();
	
	points.push_back(control[1]);
	points.push_back(control[38]);

	points.push_back(control[2]);
	points.push_back(control[5]);

	points.push_back(control[6]);
	points.push_back(control[9]);

	points.push_back(control[10]);
	points.push_back(control[13]);

	points.push_back(control[14]);
	points.push_back(control[17]);

	points.push_back(control[18]);
	points.push_back(control[21]);

	points.push_back(control[22]);
	points.push_back(control[25]);

	points.push_back(control[26]);
	points.push_back(control[29]);

	points.push_back(control[30]);
	points.push_back(control[33]);

	points.push_back(control[34]);
	points.push_back(control[37]);
	

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 3 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

Bezier::~Bezier()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Bezier::draw(GLuint shaderProgram) {


	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");

	// Materials
	glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.7f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.5f, 0.4f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.5f, 0.5f, 0.4f);
	glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 30);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	// Now draw the curve. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINE_STRIP, 0, 1500);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Bezier::drawLines(GLuint shaderProgram) {

	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");

	// Materials
	glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.5f, 0.4f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.5f, 0.5f, 0.4f);
	glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 30);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	// Now draw the curve. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	for (int i = 0; i < 10; i++)
		glDrawArrays(GL_LINE_STRIP, 1500 + 2*i, 2);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Bezier::lerp(int start) {
	
	glm::mat4x3 G_bez = { control[start], control[start+1], control[start+2], control[start+3] };

	glm::mat4x3 x = G_bez * B_bez;

	for (int i = 0; i < 150; i++) {
		float t = (float)i / 150.0f;
		glm::vec4 t_vec = { t*t*t, t*t, t, 1 };
		glm::vec3 result = x * t_vec;
		points.push_back(result);
	}
}

void Bezier::relerp() {
	points.clear();

	for (int i = 0; i < 10; i++) {
		lerp(4 * i);
	}

	highest();

	points.push_back(control[1]);
	points.push_back(control[38]);

	points.push_back(control[2]);
	points.push_back(control[5]);

	points.push_back(control[6]);
	points.push_back(control[9]);

	points.push_back(control[10]);
	points.push_back(control[13]);

	points.push_back(control[14]);
	points.push_back(control[17]);

	points.push_back(control[18]);
	points.push_back(control[21]);

	points.push_back(control[22]);
	points.push_back(control[25]);

	points.push_back(control[26]);
	points.push_back(control[29]);

	points.push_back(control[30]);
	points.push_back(control[33]);

	points.push_back(control[34]);
	points.push_back(control[37]);

	glBindVertexArray(VAO);
	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 3 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


					 // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

int Bezier::highest() {
	float maximum = -FLT_MAX;
	for (int i = 0; i < 1500; i++) {
		if (points[i].y >= maximum) {
			maximum = points[i].y;
			highest_point_index = i;
		}
	}
	return highest_point_index;
}