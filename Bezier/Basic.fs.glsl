#version 420

varying vec4 v_color;
varying vec3 v_normal;
varying vec3 v_fragPos;

uniform bool u_enable3DViewport;
uniform bool u_enabledNormal;
uniform vec3 u_camPos;

void main(void) 
{
	if(u_enabledNormal)
		gl_FragColor = vec4(abs(v_normal), 1.0);
	else if (u_enable3DViewport)
	{
		vec4 fragColor = vec4(0.5, 0.5, 0.5, 1);

		//Light
		vec3 lightPos = vec3(1, -1, 0);
		vec3 dirToLight = normalize(lightPos - v_fragPos);

		//Ambiant
		vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

		//DiffuseColor
		float maxDot = max(dot(v_normal, dirToLight), 0.0);
		vec4 diffuseColor = vec4(maxDot, maxDot, maxDot, 1.0);

		//Specular
		vec3 dirToCam = normalize(u_camPos - v_fragPos);
		vec3 H = normalize(dirToLight + dirToCam);
		float spec = max(pow(dot(dirToCam, H), 2.0), 0.0);
		vec4 specularColor = vec4(spec, spec, spec, 1.0);

		gl_FragColor = (ambient + diffuseColor + specularColor) * fragColor;
	}
	else
		gl_FragColor = v_color;
}