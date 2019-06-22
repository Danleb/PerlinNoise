#version 150 core

in vec2 positionIn;
in vec2 texcoordIn;

out vec2 texcoord;

void main()
{
	texcoord = texcoordIn;
	gl_Position = vec4(positionIn, 0.0, 1.0);	
}