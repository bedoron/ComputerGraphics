#version 150


//in  vec2 vTexCoord;
in vec3 vPosition;
in mat4 model_world;
in mat4 world_camera;

//out vec2 texCoord;

void main()
{
    gl_Position.xyz = vPosition;
    gl_Position.w=1.0;
	gl_Position = model_world * gl_Position;
//    texCoord = vTexCoord;
}
