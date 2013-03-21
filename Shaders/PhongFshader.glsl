#version 140

uniform vec4 globalAmbient;
uniform vec4 positions[12];
uniform vec4 colors[12];
uniform int num_lights;

in vec3 vpos;
in vec3 normal;

in vec4 _kambiant;
in vec4 _kdiffuse;
in vec4 _kspecular;
in float _shininess;
in float zpoint;
uniform vec4 eye;
out vec4 color;

void main()
{
	vec3 n = normalize(normal);
	color = vec4(0,0,0,0);
	int i=0;
	for(;i < num_lights ; i++)
	{
		vec4 intesity = colors[i];
		vec4 position = positions[i];
		vec3 lightDir = vec3(0);
		if(position.w==0)
		{
			lightDir = position.xyz;
		}
		else
		{
			lightDir = (position.xyz-vpos);
		}
		
		vec4 diffuse = vec4(0.0);
		vec4 specular = vec4(0.0);
		
		// diffuse term
		
		float NdotL = dot(n, normalize(lightDir));
	
		if (NdotL > 0.0)
			diffuse = _kdiffuse * NdotL;
		
		vec3 h = normalize(eye.xyz-vpos)+normalize(lightDir);
		specular= _kspecular * max(pow(dot(n,normalize(h)),_shininess),0);
		vec4 k = _kambiant;
		color += (diffuse + specular)*intesity ;
	}
	color+= _kambiant + globalAmbient;
	color += color;// * 0.01 + vec4(1, 0, 0,0);
	//color = ((zpoint +1) /2) * color + (1-((zpoint +1) /2))*vec4(0.2,0.2,0.2,1);
}