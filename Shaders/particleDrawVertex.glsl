#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec4 position;

float x;

void main(void)	{
	mat4 mvp 		= projMatrix * viewMatrix * modelMatrix;
	gl_Position		= mvp *vec4(position.xyz, 1.0);
	if(gl_Position.z<0.1){
      x = 0.01;
    }else{
      x=1/gl_Position.z;
    }
    gl_PointSize = 20.0 * x;
}
