#version 150

in  vec4 vPosition;
in  vec3 vNormal;
out vec4 color;

uniform vec4 kAmbient, kDiffuse, kSpecular;
uniform float Shininess;
uniform mat4 ModelView;
uniform mat4 CameraView;
uniform vec4 CameraPosition;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * CameraView * ModelView * vPosition;	
	vec4 tempColor = vec4(1.0, 1.0, 0.0, 1.0);
	color = tempColor;
}
