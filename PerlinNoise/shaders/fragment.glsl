#version 150 core

in vec2 texcoord;

uniform sampler2D tex;

out vec4 outColor;

void main()
{
	//outColor = vec4(texcoord, 0, 0);
	outColor = texture(tex, vec2(1,1) - texcoord.xy);
}