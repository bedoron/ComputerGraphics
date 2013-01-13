#version 150

in vec4 vPosition;
in vec4 vNormal;

out vec4 color;

uniform mat4 CameraView;
uniform mat4 Projection;
uniform mat4 ModelView;

void main()
{
	gl_Position = Projection * CameraView * ModelView * vPosition;
	color = vec4(1.0,1.0,0.0,1);
}
