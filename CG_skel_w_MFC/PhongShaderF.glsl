#version 150 

struct light
{
	int type;
	vec4 intensity;
	vec4 data;
};

in vec3 verticePosition;
in vec3 normalPosition;

out vec4 color;

uniform vec4 kAmbient, kDiffuse, kSpecular;
uniform float Shininess;

uniform vec4 CameraPosition;

uniform light Lights[8];

void main()
{
	vec3 N = normalize(normalPosition);

	vec4 tempColor = vec4(0.0, 0.0, 0.0, 1.0);

	int i;
	for (i=0; i<8; i++)
	{
		if (Lights[i].type==0) //Ambient
		{
			tempColor.x += ( kAmbient.x * Lights[i].intensity.x) ;
			tempColor.y += ( kAmbient.y * Lights[i].intensity.y) ;
			tempColor.z += ( kAmbient.z * Lights[i].intensity.z) ;	
			
		}
		else if (Lights[i].type!= -1)
		{
			float d;
			vec3 l;
			
			if (Lights[i].type == 1) //Point
			{
				l= normalize(Lights[i].data.xyz - verticePosition);
			}
			if (Lights[i].type ==2) //Parallel
			{
				l = normalize(Lights[i].data.xyz);
			}

			d = dot(N,l);
			if (d>0)
			{
				tempColor.x += ( kDiffuse.x * Lights[i].intensity.x * d );
				tempColor.y += ( kDiffuse.y * Lights[i].intensity.y * d );
				tempColor.z += ( kDiffuse.z * Lights[i].intensity.z * d );
				
			}

			vec3 v = normalize (CameraPosition.xyz - verticePosition);
			vec3 r = 2 * dot(l , N) * N - l;

			float s= dot(r,v);
			if (s>0)
			{
				color = vec4(1,0,0,1);
				tempColor.x += ( kSpecular.x * Lights[i].intensity.x * pow(s, Shininess));
				tempColor.y += ( kSpecular.y * Lights[i].intensity.y * pow(s, Shininess));
				tempColor.z += ( kSpecular.z * Lights[i].intensity.z * pow(s, Shininess));
			}
		}
	}
	color = tempColor;

}