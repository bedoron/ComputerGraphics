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

uniform vec4 eye;
uniform mat4 ModelView;
out vec4 color;

void main()
{
	vec3 n = normalize(normal);
	color = vec4(0,0,0,0);
	int i=0;
	for(;i < num_lights ; i++)
	{
		vec4 intesity = colors[0];
		vec4 position = positions[i];
		vec3 lightDir = vec3(0);
		vec4 diffuse = vec4(0.0);
		vec4 specular = vec4(0.0);
		if(position.w==0)
		{
			lightDir = position.xyz;
		}
		else
		{
			lightDir = (position.xyz-vpos);
		}
		
		// diffuse term
		float NdotL = dot(n, normalize(lightDir));
	
		if (NdotL > 0.0)
			diffuse = _kdiffuse * NdotL;
		
		vec3 h = normalize(eye.xyz-vpos)+normalize(lightDir);
		specular= _kspecular * max(pow(dot(n,normalize(h)),_shininess),0);


		color = (diffuse + specular)*intesity + _kambiant + globalAmbient;
		vec4 oldColor = color;
		
		if(color.x > 0.95)
			color.x += 1;
		else if (color.x > 0.5)
			color.x += 0.6;
		else if (color.x > 0.25)
			color.x += 0.4;
		else
			color.x += 0.2;
		if(color.y > 0.95)
			color.y += 1;
		else if (color.y > 0.5)
			color.y += 0.6;
		else if (color.y > 0.25)
			color.y += 0.4;
		else
			color.y += 0.2;
		if(color.z > 0.95)
			color.z += 1;
		else if (color.z > 0.5)
			color.z += 0.6;
		else if (color.z > 0.25)
			color.z += 0.4;
		else
			color.z += 0.2;
		if(dot(normal,normalize(eye.xyz-vpos))<0.25)
			color += vec4(0,0,0,1);
	
		vec4 newcolor;
		
		
		if(specular.z<0.5) {
			newcolor = vec4(0,0,1,0); // Blue
		} else {
			newcolor = vec4(0,1,0,0); // Gree
		}
		
	}
	//color = color * 0.01 + lightSources[0].position + 
	//color += vec4(0,1,0,0);
	//lightSources[0].color;
}