#version 150 

in  vec4 vPosition;
in  vec3 vNormal;

uniform mat4 ModelView;
uniform mat4 CameraView;
uniform mat3 NormalTransformation;
uniform mat4 Projection;

out vec3 verticePosition;
out vec3 normalPosition;

void main()
{
	gl_Position = Projection*CameraView*ModelView*vPosition;

	verticePosition = (ModelView * vPosition).xyz;
	normalPosition = NormalTransformation * vNormal;
}