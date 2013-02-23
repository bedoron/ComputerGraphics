#version 150

in vec3 vpos;
in vec3 normal;

in vec4 _Kambiant;
in vec4 _Kdiffuse;
in vec4 _Kspecular;
in float _shininess;

uniform vec4 eye;

out vec4 color;

void main()
{
	vec4 intesity = vec4(1,1,1,1);
	vec4 ambiantIntesity = vec4(0,0,0,1);
	vec3 n = normalize(normal);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
		
	// diffuse term
	vec3 lightDir = normalize(vec3(1,0.1,-0.8));
	float NdotL = dot(n, lightDir);
	
	if (NdotL > 0.0)
		diffuse = _Kdiffuse * NdotL;
		
	vec3 h = normalize(eye.xyz-vpos)+normalize(lightDir);
	specular= _Kspecular * max(pow(dot(normal,normalize(h)),_shininess),0);
	vec4 k = _Kambiant;

	color = (diffuse + specular)*intesity + ambiantIntesity * _Kambiant;
	
}