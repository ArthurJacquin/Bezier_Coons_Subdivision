#version 420

varying vec4 v_color;
varying vec3 v_normal;

void main(void) 
{
	gl_FragColor = v_color;
}