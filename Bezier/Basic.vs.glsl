#version 420
attribute vec3 a_position;
attribute vec3 a_color;
attribute vec3 a_normal;

varying vec4 v_color;
varying vec3 v_normal;

uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform bool u_enable3DViewport;

void main(void) 
{ 
	v_normal = a_normal;
	v_color = vec4(a_color, 1.0); 
	
	gl_Position = u_projectionMatrix * u_viewMatrix * vec4(a_position , 1.0);
	gl_PointSize = 5.0;
}