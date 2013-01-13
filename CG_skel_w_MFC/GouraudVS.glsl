#version 150

struct light
{
	int type;
	vec4 intensity;
	vec4 data;
};

in  vec4 vPosition;
in  vec3 vNormal;
out vec4 color;

uniform vec4 kAmbient, kDiffuse, kSpecular;
uniform vec4 Emission;
uniform float Shininess;
uniform mat4 ModelView;
uniform mat4 CameraView;
uniform vec4 CameraPosition;
uniform mat3 NormalTransformation;
uniform mat4 Projection;

uniform light Lights[8];
void main()
{
	gl_Position = Projection*CameraView*ModelView*vPosition;
	vec4 tempColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 verticePosition = (ModelView * vPosition).xyz;
	vec3 normalPosition = NormalTransformation * vNormal;
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
				l= normalize(Lights[i].data.xyz - verticePosition.xyz);
			}
			if (Lights[i].type ==2) //Parallel
			{
				l = normalize(Lights[i].data.xyz);
			}

			d = dot(normalPosition,l);
			if(d>0)
			{
				tempColor.x += ( kDiffuse.x * Lights[i].intensity.x * d );
				tempColor.y += ( kDiffuse.y * Lights[i].intensity.y * d );
				tempColor.z += ( kDiffuse.z * Lights[i].intensity.z * d );
			}

			vec3 v = normalize (CameraPosition.xyz - verticePosition);
			vec3 r = 2 * dot(l , normalPosition) * normalPosition - l;

			float s= dot(r,v);
			if (s>0)
			{
				tempColor.x += ( kSpecular.x * Lights[i].intensity.x * pow(s, Shininess));
				tempColor.y += ( kSpecular.y * Lights[i].intensity.y * pow(s, Shininess));
				tempColor.z += ( kSpecular.z * Lights[i].intensity.z * pow(s, Shininess));
			}
		}
	}
	color = tempColor;
}