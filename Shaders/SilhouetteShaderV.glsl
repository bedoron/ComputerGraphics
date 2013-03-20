#version 150

in vec4 vPosition;
in vec4 vNormal;

uniform mat4 ModelView;
uniform mat4 CameraView;
uniform mat4 Projection;


void main()
{
	vec4 vtransfromedVN = vPosition + vNormal;
	vec4 N = (ModelView * vtransfromedVN)-(ModelView * vPosition);
	vec4 normal = vec4(N.x, N.y, N.z, 1.0);
	vec4 vertice = Projection * CameraView * ModelView * vPosition;

	gl_Position = vertice + 0.01 * normal;
}
