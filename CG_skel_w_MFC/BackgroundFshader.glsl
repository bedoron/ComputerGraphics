#version 150

in vec2 st;
uniform sampler2D texMap;
out vec4 color;

void main()
{
	color = texture2D(texMap,st);
	color = vec4(1,0,1,1);
}