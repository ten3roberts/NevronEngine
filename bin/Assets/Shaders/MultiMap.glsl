#VERTEX_SHADER

#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 u_proj;
uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_texCoord = texCoord;
};

#FRAGMENT_SHADER

#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;

void main()
{
	if(!gl_FrontFacing)
		color = texture(u_texture3, v_texCoord);
	else
	{
		vec3 textureMix = normalize(vec3(texture(u_texture3, v_texCoord)));
		vec4 texColor = (texture(u_texture, v_texCoord) * textureMix.x + texture(u_texture1, v_texCoord) * textureMix.y + texture(u_texture2, v_texCoord) * textureMix.z);
		color = texColor;
	}

};

