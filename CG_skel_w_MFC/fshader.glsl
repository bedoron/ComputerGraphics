#version 150 

<<<<<<< HEAD
in vec4 color;
out vec4 fcolor;
=======
#version 150

//in  vec2 texCoord;

out vec3 fColor;

//uniform sampler2D texture;

void main() 
{ 
   //fColor = textureLod( texture, texCoord, 0 );
   fColor = vec3(1,0,0);
} 
>>>>>>> refs/heads/final-hw3

void main()
{
	fcolor = color;
}
