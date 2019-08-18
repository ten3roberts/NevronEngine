#VERTEX_SHADER

#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;
out vec4 toCamera;

uniform mat4 u_proj;
uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_texCoord = texCoord;
	toCamera = u_MVP * position;
};

#FRAGMENT_SHADER

#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec4 toCamera;


uniform vec4 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_normalMap;
uniform sampler2D u_specularMap;


layout (std140, row_major) uniform Environment
{
	vec3 skyColor;
	float viewDistance;
} environment;

void main()
{
	vec4 texColor = texture(u_texture, v_texCoord);
	//color = mix(texColor, u_color, 0.5);
	color = mix(texColor * u_color, vec4(environment.skyColor, 1), normalize(toCamera));
};

