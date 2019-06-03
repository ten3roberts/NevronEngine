#VERTEX_SHADER

#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};

#FRAGMENT_SHADER

#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main()
{
	color = u_color; // vec4(0.4, 0.0, 0.7, 1.0); //vec4(0.91, 0.36, 0.06, 1.0);
};

