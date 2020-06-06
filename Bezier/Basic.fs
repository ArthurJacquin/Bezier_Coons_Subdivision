#version 420

in vec4 v_color;
in vec3 v_normal;

void main(void) 
{
	gl_FragColor = vec4(abs(v_normal), 1.0);
}