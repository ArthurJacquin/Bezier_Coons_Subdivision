#version 420
in vec3 a_position;
in vec3 a_color;
in vec3 a_normal;

out vec4 v_color;
out vec3 v_normal;

void main(void) 
{ 
	v_normal = a_normal;
	v_color = vec4(a_color, 1.0); 

	gl_Position = vec4(a_position, 1.0);
	gl_PointSize = 5.0;
}