#version 330 core
// This is a sample fragment shader.
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 Position;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

// Uniform
uniform uint id;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
	color = vec4(id / 255.0f, 0.0f, 0.0f, 1.0f); //id / 255.0f
}
