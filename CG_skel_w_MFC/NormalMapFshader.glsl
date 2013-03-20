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
<<<<<<< HEAD
uniform sampler2D texMap;
out vec4 color;

void main()
{
	
	vec4 textureNormal = texture2D(texMap,st);
	vec3 n = normalize(2*textureNormal.xyz-1);
	
	vec4 intesity = vec4(1,1,1,1);
	n = normalize( n + normal);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
		
	// diffuse term
	vec3 lightDir = normalize(vec3(1,0.1,0.8));
	float NdotL = dot(n, lightDir);
	
	if (NdotL > 0.0)
		diffuse = _kdiffuse * NdotL;
		
	vec3 h = normalize(eye.xyz-vpos);

	specular= _kspecular * max(pow(dot(n,normalize(h)),_shininess),0);
	

	color = (diffuse+specular)*intesity;
	
=======
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
	color = vec4(0,0,0,0);
	int i=0;
	
	vec4 tColor = texture2D(colorMap,st);
	vec4 textureNormal = texture2D(normalMap,st);
	vec3 n = normalize(2*textureNormal.xyz-1);
	for(;i < 12 ; i++)
	{
		vec4 intesity = lightSources[i].color;
		vec4 position =  lightSources[i].position;
		vec3 lightDir = vec3(0);
		if(position.w==0)
		{
			lightDir = position.xyz;
		}
		else
		{
			lightDir = (position.xyz-vpos);
		}
		n = normalize( n + normal);
		vec4 diffuse = vec4(0.0);
		vec4 specular = vec4(0.0);
		
		// diffuse term
		float NdotL = dot(n, lightDir);

		if (NdotL > 0.0)
			diffuse = _kdiffuse * NdotL;
		
		vec3 h = normalize(eye.xyz-vpos);

		specular= _kspecular * max(pow(dot(n,normalize(h)),_shininess),0);
	
		color += (diffuse+specular)*intesity*tColor;
	}
	color+= _kambiant + globalAmbient;
>>>>>>> refs/heads/final-hw3
}