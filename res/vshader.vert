#version 330
layout (location = 0) in vec2 posn;
layout (location = 1) in vec3 color;

out vec3 Fcolor;

void main()
{
	gl_Position = vec4(posn,0.0,1.0);
	Fcolor = color;
}