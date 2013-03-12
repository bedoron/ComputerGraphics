#version 150

in vec3 vpos;
in vec3 normal;
in vec2 st;


uniform mat4 ModelView;
uniform vec4 eye;
uniform sampler2D normalMap;
uniform sampler2D colorMap;
out vec4 color;

void main()
{
	vec3 n = normalize(normal + normalize(2*texture2D(normalMap,st).xyz-1));	
	vec3 h = normalize(eye.xyz-vpos);
	vec3 r = reflect(n,h);
	color = texture2D(colorMap,r.xy);	
}