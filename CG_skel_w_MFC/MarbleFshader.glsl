#version 150

in vec3 vpos;
in vec3 normal;

in vec4 _kambiant;
in vec4 _kdiffuse;
in vec4 _kspecular;
in float _shininess;

uniform vec4 eye;

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



float turbulence(float p_location)
{
	float noise=0.0;
	float scale=1.0;
	while(scale>0.0001)
	{
		noise=noise+scale*sin(p_location/scale);
		scale=scale/2.0;
	}
	return noise;
}
void main()
{
	vec4 intesity = vec4(1,1,1,1);
	vec3 n = normalize(normal);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
		
	// diffuse term
	vec3 lightDir = normalize(vec3(1,0.1,-0.8));
	float NdotL = dot(n, lightDir);
	
	if (NdotL > 0.0)
		diffuse = _kdiffuse * NdotL;
		
	vec3 h = normalize(eye.xyz-vpos)+normalize(lightDir);
	specular= _kspecular * max(pow(dot(normal,normalize(h)),_shininess),0);
	float noise = turbulence(vpos.x )+ sqrt(vpos.x * vpos.x + vpos.y * vpos.y);
	vec3 marColor =noise * vec3(139,69,15)/255.0+(1-noise)*vec3(10*sin(vpos/4))/255	;
	color = vec4(marColor,1) + globalAmbient;
}