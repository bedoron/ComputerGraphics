#version 150

in vec3 vpos;
in vec3 normal;
in vec4 _kambiant;
in vec4 _kdiffuse;
in vec4 _kspecular;
in float _shininess;
in vec2 st;
uniform vec4 eye;
uniform sampler2D texMap;
out vec4 color;

void main()
{
	vec4 intesity = vec4(1,1,1,1);
	vec3 n = normalize(normal);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
		
	// diffuse term
	vec3 lightDir = normalize(vec3(1,0.1,0.8));
	float NdotL = dot(n, lightDir);
	
	if (NdotL > 0.0)
		diffuse = _kdiffuse * NdotL;
		
	vec3 h = normalize(eye.xyz-vpos)+normalize(lightDir);
	specular= _kspecular * max(pow(dot(n,normalize(h)),_shininess),0);

	vec4 textureColor = texture2D(texMap,st);
	
	color = (diffuse+specular)*intesity*textureColor;
	
}