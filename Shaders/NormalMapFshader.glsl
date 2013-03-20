#version 140

uniform vec4 globalAmbient;
uniform vec4 positions[12];
uniform vec4 colors[12];
uniform int num_lights;

in vec3 vpos;
in vec3 normal;
in vec2 st;
in vec4 _kambiant;
in vec4 _kdiffuse;
in vec4 _kspecular;
in float _shininess;

uniform mat4 ModelView;
uniform vec4 eye;
uniform sampler2D normalMap;
uniform sampler2D colorMap;
out vec4 color;

void main()
{
	color = vec4(0,0,0,0);
	int i=0;
	
	vec4 tColor = texture2D(colorMap,st);
	vec4 textureNormal = texture2D(normalMap,st);
	vec3 n = normalize(2*textureNormal.xyz-1);
	for(;i < num_lights ; i++)
	{
		vec4 intesity = colors[i];
		vec4 position =  positions[i];
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
		float NdotL = dot(n, normalize(lightDir));

		if (NdotL > 0.0)
			diffuse = _kdiffuse * NdotL;
		
		vec3 h = normalize(eye.xyz-vpos);

		specular= _kspecular * max(pow(dot(n,normalize(h)),_shininess),0);
	
		color += (diffuse+specular)*intesity*tColor;
	}
	color+= _kambiant + globalAmbient;
	//color = color * 0.01 +positions[0];
}