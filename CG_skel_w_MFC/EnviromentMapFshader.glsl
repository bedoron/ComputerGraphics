#version 150

in vec3 vpos;
in vec3 normal;
in vec2 st;
<<<<<<< HEAD
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
=======


uniform mat4 ModelView;
uniform vec4 eye;
uniform sampler2D normalMap;
uniform sampler2D colorMap;
out vec4 color;

struct LightSource {
	vec4 position;
	vec4 color;
};

layout(std140) uniform LightSourcesBlock  {
	vec4		globalAmbient;
	LightSource lightSources[12];
	int		num_lights;
};




void main()
{
	vec3 n = normalize(normal + normalize(2*texture2D(normalMap,st).xyz-1));	

	vec3 u = normalize(eye.xyz-vpos);
//	vec2 r = (vec3(0.5) + normalize(reflect(n,u))/1.41421).xy;
	vec3 r = reflect( n, u );
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	color = texture2D(colorMap,vec2(r.x/m + 0.5,r.y/m + 0.5))	;
>>>>>>> refs/heads/final-hw3
}