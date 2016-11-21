#include "window.h"

const char* window_title = "Project 4";

// Initializations
Skybox * skybox;
Bezier * curve;
Sphere * sphere[40];
SphereENV * env;
OBJObject2 * pod;
GLuint skyboxTexture;

// On/off vars
bool showSpheres = true;
bool PAUSE = false;
bool DEBUG = false;
int env_location = 0;
int id_selected = -1;
bool BUFFER = false;
bool POD = false;
bool MOVEX = false;
bool MOVEY = false;

GLint skybox_shaderProgram;
GLint object_shaderProgram;
GLint env_shaderProgram;
GLint selection_buffer;

// On some systems you need to change this to the absolute path
#define SKYBOX_VERTEX_SHADER_PATH "../skybox.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skybox.frag"
#define OBJECT_VERTEX_SHADER_PATH "../object.vert"
#define OBJECT_FRAGMENT_SHADER_PATH "../object.frag"
#define ENV_VERTEX_SHADER_PATH "../env.vert"
#define ENV_FRAGMENT_SHADER_PATH "../env.frag"
#define SELECTION_VERTEX_SHADER_PATH "../select.vert"
#define SELECTION_FRAGMENT_SHADER_PATH "../select.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera, z = 20.0f default, reset 1.0f
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at, z = 0.0f default
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

// Callback variables
bool click = true;
float cursor_x = 0;
float cursor_y = 0;
float cursor_x_old = 0;
float cursor_y_old = 0;
glm::vec3 old_location;
bool lmb = false;
bool rmb = false;
bool button_down = false;

void Window::initialize_objects()
{
	// Initialize camera
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	// And skybox
	skybox = new Skybox(200);
	skyboxTexture = skybox->loadCubemap();

	// Initialize curve
	curve = new Bezier();

	pod = new OBJObject2("pod.obj");

	// Sphere 
	for (int i = 0; i < 40; i++) {
		sphere[i] = new Sphere(i);
		sphere[i]->shaderProgram = object_shaderProgram;
	}

	env = new SphereENV();
	env->toWorld = glm::translate(glm::mat4(1.0f), curve->points[curve->highest()]);
	env_location = curve->highest();

	// Load the shader program. Make sure you have the correct filepath up top
	skybox_shaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	object_shaderProgram = LoadShaders(OBJECT_VERTEX_SHADER_PATH, OBJECT_FRAGMENT_SHADER_PATH);
	env_shaderProgram = LoadShaders(ENV_VERTEX_SHADER_PATH, ENV_FRAGMENT_SHADER_PATH);
	selection_buffer = LoadShaders(SELECTION_VERTEX_SHADER_PATH, SELECTION_FRAGMENT_SHADER_PATH);

	
	env->shaderProgram = env_shaderProgram;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	delete(env);
	delete(curve);
	for (int i = 0; i < 40; i++) {
		delete(sphere[i]);
	}

	glDeleteProgram(skybox_shaderProgram);
	glDeleteProgram(object_shaderProgram);
	glDeleteProgram(env_shaderProgram);
	glDeleteProgram(selection_buffer);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

glm::vec3 Window::trackball(float x, float y)    // Use separate x and y values for the mouse location
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (2.0f*x - Window::width) / Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0f*y) / Window::height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
	d = (float)v.length();    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d<1.0) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = (float)sqrtf(1.001 - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	v = normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}




/************************************
*		START NEW CALLBACKS			*
*									*
*************************************/



void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	cursor_x = xpos;
	cursor_y = ypos;
	if (click == true) {
		cursor_x_old = xpos;
		cursor_y_old = ypos;
		click = false;
	}

	// Rotate camera
	if (lmb && button_down) {
		glm::vec3 location = trackball(cursor_x, cursor_y);
		glm::vec3 direction = location - old_location;
		float velocity = direction.length();

		// Velocity check
		if (velocity > 0.0001) {
			glm::vec3 rot_axis;
			rot_axis = cross(old_location, location);
			velocity = velocity / (3.25);
			rot_axis[2] = 0.0f;

			cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), velocity / 180.0f * glm::pi<float>(), rot_axis) * glm::vec4(cam_pos, 1));
			cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), velocity / 180.0f * glm::pi<float>(), rot_axis) * glm::vec4(cam_up, 1));

			V = glm::lookAt(cam_pos, cam_look_at, cam_up);

		}
	}
	else if (rmb && button_down && id_selected != -1) {
		glm::vec3 location = trackball(cursor_x, cursor_y);
		glm::vec3 direction = location - old_location;
		float velocity = direction.length();
		//glm::vec3 vector2 = cross(curve->control[id_selected] - cam_pos, direction);
		float varx = 0.0f;
		float vary = 0.0f;
		
		
		if (direction.x > 0) {
			varx = 500.0f;
		}
		else {
			varx = -500.0f;
		}

		if (direction.y > 0) {
			vary = 25.0f;
		}
		else {
			vary = -25.0f;
		}

		// Velocity check
		if (velocity > 0.0001) {
			velocity = velocity / (3.25);

			

			direction.x = cross(cam_look_at - cam_pos, cam_up).x / (varx);
			direction.y = cam_up.y / (vary);
			direction.z = 0.0f;

			if (MOVEX) {
				direction.y = 0.0f;
			}
			else if (MOVEY) {
				direction.x = 0.0f;
			}

			if (cam_pos.z < 10 && cam_pos.z > -10) {
				direction.x = 0.0f;
				//direction.y = 0.0f;
				direction.z = cross(cam_look_at - cam_pos, cam_up).x / (varx / 10.0f);
				if (MOVEY)
					direction.z = 0.0f;
				if (cam_pos.z < 0) {
					direction.z = -direction.z;
				}
			}

			sphere[id_selected]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z));
			curve->control[id_selected] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[id_selected], 1));

			// Checks for beginning / end points
			if (id_selected % 4 == 0)
			{
				if (id_selected == 0) {
					sphere[39]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[39]->toWorld;
					curve->control[39] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[39], 1));
					sphere[38]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[38]->toWorld;
					curve->control[38] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[38], 1));
					sphere[1]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[1]->toWorld;
					curve->control[1] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[1], 1));
				}
				else {
					sphere[id_selected - 1]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[id_selected - 1]->toWorld;
					curve->control[id_selected - 1] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[id_selected - 1], 1));
					sphere[id_selected - 2]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[id_selected - 2]->toWorld;
					curve->control[id_selected - 2] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[id_selected - 2], 1));
					sphere[id_selected + 1]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[id_selected + 1]->toWorld;
					curve->control[id_selected + 1] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[id_selected + 1], 1));
				}
			}
			else if (id_selected % 4 == 3)
			{
				if (id_selected == 39) {
					sphere[0]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[0]->toWorld;
					curve->control[0] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[0], 1));
					sphere[1]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[1]->toWorld;
					curve->control[1] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[1], 1));
					sphere[38]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[38]->toWorld;
					curve->control[38] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[38], 1));
				}
				else {
					sphere[id_selected + 1]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[id_selected + 1]->toWorld;
					curve->control[id_selected + 1] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[id_selected + 1], 1));
					sphere[id_selected + 2]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[id_selected + 2]->toWorld;
					curve->control[id_selected + 2] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[id_selected + 2], 1));
					sphere[id_selected - 1]->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(direction.x, direction.y, direction.z)) * sphere[id_selected - 1]->toWorld;
					curve->control[id_selected - 1] = glm::vec3(glm::translate(glm::mat4(1.0f), direction) * glm::vec4(curve->control[id_selected - 1], 1));
				}
			}
			else if (id_selected % 4 == 1) {
				if (id_selected == 1) {
					//glm::vec3 mid = glm::vec3(2.0f * curve->control[0] - curve->control[1]);
					sphere[38]->toWorld = glm::translate(glm::mat4(1.0f), -direction) * sphere[38]->toWorld;
					curve->control[38] = glm::vec3(glm::translate(glm::mat4(1.0f), -direction) * glm::vec4(curve->control[38], 1));
				}
				else {
					//glm::vec3 mid = glm::vec3(2.0f * curve->control[id_selected - 1] - curve->control[id_selected]);
					sphere[id_selected - 3]->toWorld = glm::translate(glm::mat4(1.0f), -direction) * sphere[id_selected - 3]->toWorld;
					curve->control[id_selected - 3] = glm::vec3(glm::translate(glm::mat4(1.0f), -direction) * glm::vec4(curve->control[id_selected - 3], 1));
				}
			}
			else if (id_selected % 4 == 2) {
				if (id_selected == 38) {
					//glm::vec3 mid = glm::vec3(2.0f * curve->control[0] - curve->control[38]);
					sphere[1]->toWorld = glm::translate(glm::mat4(1.0f), -direction) * sphere[1]->toWorld;
					curve->control[1] = glm::vec3(glm::translate(glm::mat4(1.0f), -direction) * glm::vec4(curve->control[1], 1));
				}
				else {
					//glm::vec3 mid = glm::vec3(2.0f * curve->control[id_selected + 1] - curve->control[id_selected]);
					sphere[id_selected + 3]->toWorld = glm::translate(glm::mat4(1.0f), -direction) * sphere[id_selected + 3]->toWorld;
					curve->control[id_selected + 3] = glm::vec3(glm::translate(glm::mat4(1.0f), -direction) * glm::vec4(curve->control[id_selected + 3], 1));
				}
			}

			curve->relerp();
			//* sphere[id_selected]->toWorld;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		button_down = true;
		click = true;
		old_location = trackball(cursor_x, cursor_y);

		if (button == GLFW_MOUSE_BUTTON_1) {
			lmb = true;
			rmb = false;
		}

		if (button == GLFW_MOUSE_BUTTON_2) {
			lmb = false;
			rmb = true;
			BUFFER = true;
		}
	}
	else {
		button_down = false;
		lmb = false;
		rmb = false;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset != 0) {
		cam_pos.z += -yoffset;
		if (cam_pos.z == 0) {
			cam_pos.z += -yoffset;
		}
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	}
}


/************************************
*		END NEW CALLBACKS			*
*									*
*************************************/


void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//env->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (DEBUG == false && BUFFER == false) {
		// Use the shader of programID, skybox
		glUseProgram(skybox_shaderProgram);
		skybox->draw(skybox_shaderProgram);



		// Use object shader
		glDisable(GL_CULL_FACE);

		glUseProgram(env_shaderProgram);
		float velocity = sqrt(abs(-2 * 0.2 * (curve->points[env_location].y - curve->points[curve->highest()].y))) + 1;
		if (PAUSE == true) {
			velocity = 0;
		}

		env_location = env_location + velocity;

		if (env_location >= 1500) {
			env_location = 0;
		}

		env->toWorld = glm::translate(glm::mat4(1.0f), curve->points[env_location]);
		pod->toWorld = glm::translate(glm::mat4(1.0f), curve->points[env_location]) * pod->toWorld;
		
		if (POD == false) {
			env->solidSphere(skybox_shaderProgram, cam_pos, skyboxTexture);
		}

		// Send light info
		glUseProgram(object_shaderProgram);
		skybox->sendLight(object_shaderProgram);
		if (POD == true) {
			pod->draw(object_shaderProgram);
		}
		curve->draw(object_shaderProgram);
		curve->drawLines(object_shaderProgram);


		// Draw spheres
		if (showSpheres) {
			for (int i = 0; i < 40; i++) {
				sphere[i]->toWorld = glm::translate(glm::mat4(1.0f), curve->control[i]);
				if (i % 4 == 1 || i % 4 == 2) {
					sphere[i]->solidSphere(object_shaderProgram, true);
				}
				else {
					sphere[i]->solidSphere(object_shaderProgram, false);
				}
			}
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else if (DEBUG == true) {
		glDisable(GL_CULL_FACE);
		glUseProgram(selection_buffer);
		for (int i = 0; i < 40; i++) {
			sphere[i]->selection_buffer(selection_buffer);
		}
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else if (BUFFER == true) {
		glUseProgram(selection_buffer);
		for (int i = 0; i < 40; i++) {
			sphere[i]->selection_buffer(selection_buffer);
		}

		unsigned char pix[4];
		glReadPixels(cursor_x, height - cursor_y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
		id_selected = pix[0];
		if (id_selected == 51) {
			id_selected = -1;
		}
		std::cout << "Selected: " << id_selected << std::endl;
		BUFFER = false;
	}
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_B) {
			if (showSpheres == true) {
				showSpheres = false;
			}
			else {
				showSpheres = true;
			}
		}

		else if (key == GLFW_KEY_X) {
			if (MOVEX == true) {
				MOVEX = false;
			}
			else {
				MOVEX = true;
				MOVEY = false;
			}
		}

		else if (key == GLFW_KEY_C) {
			if (MOVEY == true) {
				MOVEY = false;
			}
			else {
				MOVEY = true;
				MOVEX = false;
			}
		}

		else if (key == GLFW_KEY_P) {
			if (PAUSE == true) {
				PAUSE = false;
			}
			else {
				PAUSE = true;
			}
		}

		else if (key == GLFW_KEY_D) {
			if (DEBUG == true) {
				DEBUG = false;
			}
			else {
				DEBUG = true;
			}
			for (int i = 0; i < 40; i++) {
				std::cout << i << ": " << curve->control[i].x << ", " << curve->control[i].y << ", " << curve->control[i].z << std::endl;
			}
		}

		else if (key == GLFW_KEY_T) {
			if (POD == true) {
				POD = false;
			}
			else {
				POD = true;
			}
		}

		else if (key == GLFW_KEY_O) {
			int index = curve->highest();
			glm::vec3 point = curve->points[index];
			env_location = index;
			env->toWorld = glm::translate(glm::mat4(1.0f), point);
		}

		// RESET
		else if (key == GLFW_KEY_R) {
			cam_pos = glm::vec3(0.0f, 00.0f, 20.0f);
			cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
			cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
	}
}