#version 150 

in vec4 vPosition;
in vec4 vNormal;

in vec4 kambiant;
in vec4 kdiffuse;
in vec4 kspecular;
in float shininess;

uniform float time;
uniform mat4 Projection;
uniform mat4 CameraView;
uniform mat4 ModelView;

out vec3 vpos;
out vec3 normal;
out vec4 _kambiant;
out vec4 _kdiffuse;
out vec4 _kspecular;
out float _shininess;

void main()
{
	mat4 NormalTranform = ModelView;
	NormalTranform[3][0] = 0;
	NormalTranform[3][1] = 0;
	NormalTranform[3][2] = 0;
	if(time>0)
		gl_Position =  Projection * CameraView * ModelView * (vPosition + vNormal * sin(time));
	else
		gl_Position =  Projection * CameraView * ModelView * vPosition;
	
	vec4 vtransfromedVN = vPosition + vNormal;
	vec4 vNormalOut = (ModelView * vtransfromedVN)-(ModelView * vPosition);
	normal = normalize(vNormalOut.xyz/vNormalOut.w);
	vpos = (ModelView * vPosition).xyz;
	_kambiant = kambiant;
	_kdiffuse = kdiffuse;
	_kspecular = kspecular;
	_shininess = shininess;
} 