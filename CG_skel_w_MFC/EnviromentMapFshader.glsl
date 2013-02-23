#version 150

in vec3 vpos;
in vec3 normal;
in vec2 st;
in vec4 _kambiant;
in vec4 _kdiffuse;
in vec4 _kspecular;
in float _shininess;

uniform mat4 ModelView;
uniform vec4 eye;
uniform sampler2D texMap;
uniform sampler2D envMap;
out vec4 color;

void main()
{
	vec3 n = normalize(normal + 2*texture2D(texMap,st).xyz-1);	
	vec3 h = normalize(eye.xyz-vpos);
	vec3 r = reflect(n,h);
	color = texture2D(envMap,r.xy);	
}