#version 150 

in vec2 vPosition;
in vec2 tCoor;

out vec2 st;

void main()
{
	gl_Position =  vec4( vPosition,0,1);
	st = tCoor;
} 