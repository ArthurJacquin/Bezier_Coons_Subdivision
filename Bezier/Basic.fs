#version 420

varying vec4 v_color;
varying vec3 v_normal;

void main(void) 
{
//vec4(abs(v_normal), 1.0)
	gl_FragColor = v_color;
}